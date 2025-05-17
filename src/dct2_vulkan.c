#ifdef HAS_VULKAN
#include "dct2.h"
#include "shaders/shaders.h"

#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <vulkan/vulkan.h>

typedef struct ShaderPrograms {
  VkShaderModule shaderDCT2DHorizontal;
  VkShaderModule shaderDCT2DVertical;

  VkDescriptorSetLayout setLayout;
  VkPipelineLayout layout;

  VkPipeline pipelineDCT2DHorizontal;
  VkPipeline pipelineDCT2DVertical;
} ShaderPrograms;

typedef struct dct_context {
  VkInstance instance;

  VkPhysicalDevice gpu;
  uint32_t computeQueueIndex;

  VkDevice device;
  VkQueue queue;

  VkCommandPool commandPool;
  VkDescriptorPool descriptorPool;

  ShaderPrograms programs;
  VkCommandBuffer cmd;

  size_t max_n;    // size of the largest transform we'll do
  double* scratch; // single buffer of length max_n
} dct_context;

typedef struct PushConstants {
  uint32_t width;
  uint32_t height;
  uint32_t isInverse;
} PushConstants;

const size_t VALIDATION_LAYERS_COUNT = 1;
const char* VALIDATION_LAYERS[VALIDATION_LAYERS_COUNT] = {
  "VK_LAYER_KHRONOS_validation"
};
const size_t REQUIRED_EXTENSIONS_COUNT = 1;
const char* REQUIRED_EXTENSIONS[REQUIRED_EXTENSIONS_COUNT] = {
  "VK_KHR_portability_enumeration",
};

const size_t REQUIRED_DEVICE_EXTENSIONS_COUNT = 1;
const char* REQUIRED_DEVICE_EXTENSIONS[REQUIRED_DEVICE_EXTENSIONS_COUNT] = {
  "VK_KHR_portability_subset",
};

DCT_API dct_context* dct_context_alloc(size_t max_n) {
  dct_context* ctx = (dct_context*)malloc(sizeof(dct_context));
  if (!ctx) return NULL;
  ctx->max_n = max_n;
  ctx->scratch = (double*)malloc(sizeof(double) * max_n);
  ctx->programs = (ShaderPrograms){ 0 };
  if (!ctx->scratch) {
    free(ctx);
    return NULL;
  }
  return ctx;
}

void dct_context_free(dct_context* ctx) {
  if (!ctx) return;
  free(ctx->scratch);
  free(ctx);
}

int dct_init(dct_context* ctx) {

  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "DCT2D Library",      // appName
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",      // engineName
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_2,
  };

  VkInstanceCreateInfo istCInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = VALIDATION_LAYERS_COUNT,
    .ppEnabledLayerNames = VALIDATION_LAYERS,
    .enabledExtensionCount = REQUIRED_EXTENSIONS_COUNT,
    .ppEnabledExtensionNames = REQUIRED_EXTENSIONS,
    .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
  };

  VkResult res = vkCreateInstance(&istCInfo, NULL, &ctx->instance);

  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create instance (%d)\n", res);
    return -1;
  }

  // Get GPU
  uint32_t devCount;
  res = vkEnumeratePhysicalDevices(ctx->instance, &devCount, NULL);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to enumerate Physical Devices (%d)\n", res);
    return -1;
  }

  if (devCount <= 0) {
    fprintf(stderr, "No GPU found!\n");
    return -2;
  }
  fprintf(stderr, "Found %u GPUs\n", devCount);

  VkPhysicalDevice devices[devCount];
  vkEnumeratePhysicalDevices(ctx->instance, &devCount, devices);

  ctx->gpu = devices[0];
  for (uint32_t i = 0; i < devCount; i++) {
    VkPhysicalDeviceProperties2 props = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
    };
    vkGetPhysicalDeviceProperties2(devices[i], &props);

    fprintf(stderr, "Detected Device: %s (Vulkan API: %u.%u.%u)\n", props.properties.deviceName, VK_VERSION_MAJOR(props.properties.apiVersion), VK_VERSION_MINOR(props.properties.apiVersion), VK_VERSION_PATCH(props.properties.apiVersion));
    if (props.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      fprintf(stderr, "Selected as discrete GPU: %s\n", props.properties.deviceName);
      ctx->gpu = devices[i];
      break;
    }
  }

  // Verify GPU can compute
  uint32_t qfCount;
  vkGetPhysicalDeviceQueueFamilyProperties(ctx->gpu, &qfCount, NULL);

  VkQueueFamilyProperties families[qfCount];
  vkGetPhysicalDeviceQueueFamilyProperties(ctx->gpu, &qfCount, families);

  for (uint32_t i = 0; i < qfCount; i++) {
    if (families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
      ctx->computeQueueIndex = i;
    }
  }

  // Create logical device
  float priority = 1.0f;
  VkDeviceQueueCreateInfo queueInfo = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .queueFamilyIndex = ctx->computeQueueIndex,
    .queueCount = 1,
    .pQueuePriorities = &priority,
  };

  VkPhysicalDeviceFeatures deviceFeatures = { 0 };
  VkDeviceCreateInfo deviceInfo = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &queueInfo,
    .enabledExtensionCount = REQUIRED_DEVICE_EXTENSIONS_COUNT,
    .ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .pEnabledFeatures = &deviceFeatures,
  };

  res = vkCreateDevice(ctx->gpu, &deviceInfo, NULL, &ctx->device);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create device (%d)\n", res);
    return -1;
  }

  // Get queue
  vkGetDeviceQueue(ctx->device, ctx->computeQueueIndex, 0, &ctx->queue);

  // Create pools
  // Create command pool
  VkCommandPoolCreateInfo commandPoolInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .queueFamilyIndex = ctx->computeQueueIndex,
  };
  res = vkCreateCommandPool(ctx->device, &commandPoolInfo, NULL, &ctx->commandPool);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create command pool (%d)\n", res);
    return -1;
  }

  VkCommandBufferAllocateInfo commandBufAllocInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = ctx->commandPool,
    .commandBufferCount = 1,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
  };

  res = vkAllocateCommandBuffers(ctx->device, &commandBufAllocInfo, &ctx->cmd);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to allocate command buffers (%d)\n", res);
    return -1;
  }

  VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      10 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,       10 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
  };
  VkDescriptorPoolCreateInfo descriptorPoolInfo = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    .maxSets = 10,
    .poolSizeCount = 3,
    .pPoolSizes = poolSizes,
  };

  res = vkCreateDescriptorPool(ctx->device, &descriptorPoolInfo, NULL, &ctx->descriptorPool);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create descriptor pool (%d)\n", res);
    return -1;
  }

  const uint32_t pipelineCount = 2;
  VkShaderModuleCreateInfo shaderInfos[pipelineCount] = {
    {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pCode = (uint32_t*)dct_horiz_comp,
    .codeSize = dct_horiz_comp_size,
  },
    {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pCode = (uint32_t*)dct_vert_comp,
    .codeSize = dct_vert_comp_size,
  },
  };

  res = vkCreateShaderModule(ctx->device, shaderInfos, NULL, &ctx->programs.shaderDCT2DHorizontal);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create Horizontal shader module (%d)\n", res);
    return -1;
  }

  res = vkCreateShaderModule(ctx->device, shaderInfos + 1, NULL, &ctx->programs.shaderDCT2DVertical);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create Vertical shader module (%d)\n", res);
    return -1;
  }

  // layout(push_constant) uniform PC {
  //   int width;
  //   int height;
  //   int isInverse;    // 0 = forward, 1 = inverse
  // } pc;
  VkPushConstantRange pushConstants = {
    .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
    .offset = 0,
    .size = sizeof(PushConstants),
  };

  const uint32_t bindingCount = 2;
  VkDescriptorSetLayoutBinding bindings[bindingCount] = {
    {
      .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
      .binding = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    },
    {
      .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
      .binding = 1,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    },
  };

  VkDescriptorSetLayoutCreateInfo setLayoutInfo = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .bindingCount = bindingCount,
    .pBindings = bindings,
  };

  res = vkCreateDescriptorSetLayout(ctx->device, &setLayoutInfo, NULL, &ctx->programs.setLayout);

  VkPipelineLayoutCreateInfo layoutInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pushConstantRangeCount = 1,
    .pPushConstantRanges = &pushConstants,
    .setLayoutCount = 1,
    .pSetLayouts = &ctx->programs.setLayout,
  };

  res = vkCreatePipelineLayout(ctx->device, &layoutInfo, NULL, &ctx->programs.layout);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create pipeline layout (%d)\n", res);
    return -1;
  }

  VkComputePipelineCreateInfo pipelineCreateInfos[pipelineCount] = {
    {
      .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
      .stage = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pName = "main",
        .module = ctx->programs.shaderDCT2DHorizontal,
        .stage = VK_SHADER_STAGE_COMPUTE_BIT,
        .pSpecializationInfo = NULL,
      },
      .layout = ctx->programs.layout,
    },
    {
      .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
      .basePipelineIndex = 0,
      .stage = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pName = "main",
        .module = ctx->programs.shaderDCT2DVertical,
        .stage = VK_SHADER_STAGE_COMPUTE_BIT,
        .pSpecializationInfo = NULL,
      },
      .layout = ctx->programs.layout,
    },
  };

  res = vkCreateComputePipelines(ctx->device, VK_NULL_HANDLE, pipelineCount, pipelineCreateInfos, NULL, &ctx->programs.pipelineDCT2DHorizontal);
  if (res != VK_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to create compute pipeline (%d)\n", res);
    return -1;
  }

  return 0;
}

int dct_shutdown(dct_context* ctx) {

  vkDestroyPipeline(ctx->device, ctx->programs.pipelineDCT2DHorizontal, NULL);
  vkDestroyPipeline(ctx->device, ctx->programs.pipelineDCT2DVertical, NULL);

  vkDestroyPipelineLayout(ctx->device, ctx->programs.layout, NULL);
  vkDestroyDescriptorSetLayout(ctx->device, ctx->programs.setLayout, NULL);

  vkDestroyShaderModule(ctx->device, ctx->programs.shaderDCT2DHorizontal, NULL);
  vkDestroyShaderModule(ctx->device, ctx->programs.shaderDCT2DVertical, NULL);

  vkDestroyDescriptorPool(ctx->device, ctx->descriptorPool, NULL);
  vkDestroyCommandPool(ctx->device, ctx->commandPool, NULL);

  vkDestroyDevice(ctx->device, NULL);

  vkDestroyInstance(ctx->instance, NULL);

  return 0;
}

// 1D DCT (in‑place if out == base)
DCT_API int dct1d(dct_context* ctx,
  double* func,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !func || !out || stride == 0 || length > ctx->max_n)
    return -1;

  double alpha_0 = 1.0 / sqrt((double)length);
  double alpha_k = sqrt(2.0 / (double)length);

  double* T = ctx->scratch;
  for (size_t k = 0; k < length; ++k) {
    double sum = 0.0;
    double alpha = (k == 0) ? alpha_0 : alpha_k;
    for (size_t i = 0; i < length; ++i) {
      sum += func[i * stride]
        * cos(k * M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)length));
    }
    T[k] = alpha * sum;
  }
  // copy back (allows out == func)
  for (size_t i = 0; i < length; ++i) {
    out[i * stride] = T[i];
  }
  return 0;
}

// 1D inverse DCT
DCT_API int idct1d(dct_context* ctx,
  double* coeff,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !coeff || !out || stride == 0 || length > ctx->max_n)
    return -1;

  double alpha_0 = 1.0 / sqrt((double)length);
  double alpha_k = sqrt(2.0 / (double)length);

  double* T = ctx->scratch;
  for (size_t i = 0; i < length; ++i) {
    double sum = 0.0;
    for (size_t k = 0; k < length; ++k) {
      double alpha = (k == 0) ? alpha_0 : alpha_k;
      sum += alpha * coeff[k * stride]
        * cos(k * M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)length));
    }
    T[i] = sum;
  }
  for (size_t i = 0; i < length; ++i) {
    out[i * stride] = T[i];
  }
  return 0;
}

// 2D DCT: rows then columns, in‑place
DCT_API int dct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {

  const size_t size = width * height * sizeof(double);
  VkMemoryAllocateInfo memInfo = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .memoryTypeIndex = 0,
    .allocationSize = size,
  };
  VkDeviceMemory stagingMemory;
  vkAllocateMemory(ctx->device, &memInfo, NULL, &stagingMemory);

  void* data;
  vkMapMemory(ctx->device, stagingMemory, 0, size, 0, &data);
  memcpy(data, matrix, size);
  vkUnmapMemory(ctx->device, stagingMemory);

  VkBufferCreateInfo bufInfo = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .queueFamilyIndexCount = 1,
    .pQueueFamilyIndices = &ctx->computeQueueIndex,
    .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    .sharingMode = VK_SHARING_MODE_CONCURRENT,
    .size = size,
  };
  VkBuffer stagingBuffer;
  vkCreateBuffer(ctx->device, &bufInfo, NULL, &stagingBuffer);

  VkMemoryRequirements memReq;
  vkGetBufferMemoryRequirements(ctx->device, &stagingBuffer, &memReq);

  uint32_t memoryTypeIndex = 0;
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(ctx->gpu, &memProperties);
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((memReq.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
      memoryTypeIndex = i;
      break;
    }
  }

  VkMemoryAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memReq.size,
      .memoryTypeIndex = memoryTypeIndex
  };
  vkAllocateMemory(ctx->device, &allocInfo, NULL, &stagingMemory);
  vkBindBufferMemory(ctx->device, stagingBuffer, stagingMemory, 0);

  VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  vkBeginCommandBuffer(ctx->cmd, &beginInfo);

  VkBufferCopy copyRegion = {
    .srcOffset = 0,
    .dstOffset = 0,
    .size = size,
  }; // TODO: VkImageBufferCopy
  vkCmdCopyBufferToImage(ctx->cmd, &stagingBuffer, NULL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, &copyRegion); // TODO: sistemare i buffer e img

  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: LA ROBA SOPRA UN PO MERDA
  // TODO: Allocate an in/out image buffer HOSTVISIBLE | HOST COHERENT | DEVICE LOCAL
  // TODO: Allocate a temp image buffer DEVICE LOCAL
  // TODO: Copy from matrix to image buffer
  // TODO: Exec
  // TODO: Copy from image buffer to matrix

  vkCmdBindPipeline(ctx->cmd, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->programs.pipelineDCT2DHorizontal);
  // vkCmdBindDescriptorSets(ctx->cmd, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->programs.layout, 0, 1, &ctx->programs.firstPassDescriptor, 0, NULL);

  PushConstants pc = {
    .width = width,
    .height = height,
    .isInverse = 0,
  };
  vkCmdPushConstants(ctx->cmd, ctx->programs.layout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(pc), &pc);

  vkCmdDispatch(ctx->cmd, (uint32_t)ceil(width / 16.0), (uint32_t)height, 1); // TODO: set right counts

  VkImageMemoryBarrier barr = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_GENERAL,
    .newLayout = VK_IMAGE_LAYOUT_GENERAL,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = NULL, // TODO: tmpImg
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .levelCount = 1,
      .layerCount = 1,
    },
  };
  vkCmdPipelineBarrier(ctx->cmd, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barr);

  vkCmdBindPipeline(ctx->cmd, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->programs.pipelineDCT2DVertical);
  // vkCmdBindDescriptorSets(ctx->cmd, VK_PIPELINE_BIND_POINT_COMPUTE, ctx->programs.layout, 0, 1, &ctx->programs.secondPassDescriptor, 0, NULL);

  vkCmdPushConstants(ctx->cmd, ctx->programs.layout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(pc), &pc);
  vkCmdDispatch(ctx->cmd,
    (uint32_t)width,
    (uint32_t)ceil(height / 16.0),
    1); // TODO set right counts

  barr.image = NULL; // TODO dstImage
  barr.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
  barr.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  vkCmdPipelineBarrier(ctx->cmd, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barr);

  // TODO: copy dst image to matrix

  vkEndCommandBuffer(ctx->cmd);

  VkSubmitInfo submitInfo = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 0,
    .pWaitSemaphores = NULL,
    .pWaitDstStageMask = NULL,
    .commandBufferCount = 1,
    .pCommandBuffers = &ctx->cmd,
    .signalSemaphoreCount = 0,
    .pSignalSemaphores = NULL,
  };

  vkQueueSubmit(ctx->queue, 1, &submitInfo, NULL);
  vkQueueWaitIdle(ctx->queue);

  vkResetCommandBuffer(ctx->cmd, 0);

  return 0;
}

// 2D inverse DCT: rows then columns, in‑place
DCT_API int idct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {
  if (!ctx || !matrix || width * height > ctx->max_n * ctx->max_n)
    return -1;

  // Inverse DCT each row
  for (size_t r = 0; r < height; ++r) {
    double* row = matrix + r * width;
    if (idct1d(ctx, row, width, 1, row) < 0)
      return -1;
  }

  // Inverse DCT each column
  for (size_t c = 0; c < width; ++c) {
    double* col = matrix + c;
    if (idct1d(ctx, col, height, width, col) < 0)
      return -1;
  }

  return 0;
}

void _adj_block(size_t width, size_t height,
  size_t blk_size,
  size_t* out_bw, size_t* out_bh) {
  // numero di blocchi in ciascuna direzione
  size_t nx = (width + blk_size - 1) / blk_size;
  size_t ny = (height + blk_size - 1) / blk_size;
  // dimensione effettiva dei blocchi (ceil division)
  *out_bw = (width + nx - 1) / nx;
  *out_bh = (height + ny - 1) / ny;
}


DCT_API int dct2dblkrounded(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  size_t BW, BH;
  _adj_block(width, height, blk_size, &BW, &BH);

  if (BW > ctx->max_n || BH > ctx->max_n) return -1;

  // ora dividiamo in blocchi BW×BH
  for (size_t by = 0; by < height; by += BH) {
    size_t h = (by + BH <= height ? BH : height - by);
    for (size_t bx = 0; bx < width; bx += BW) {
      size_t w = (bx + BW <= width ? BW : width - bx);

      // DCT sulle righe del blocco
      for (size_t i = 0; i < h; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (dct1d(ctx, row, w, 1, row) < 0)
          return -1;
      }
      // DCT sulle colonne del blocco
      for (size_t j = 0; j < w; ++j) {
        double* col = mat + by * width + (bx + j);
        if (dct1d(ctx, col, h, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}

// iDCT 2D a blocchi “arrotondati”
DCT_API int idct2dblkrounded(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  size_t BW, BH;
  _adj_block(width, height, blk_size, &BW, &BH);

  if (BW > ctx->max_n || BH > ctx->max_n) return -1;

  for (size_t by = 0; by < height; by += BH) {
    size_t h = (by + BH <= height ? BH : height - by);
    for (size_t bx = 0; bx < width; bx += BW) {
      size_t w = (bx + BW <= width ? BW : width - bx);

      // iDCT sulle righe del blocco
      for (size_t i = 0; i < h; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (idct1d(ctx, row, w, 1, row) < 0)
          return -1;
      }
      // iDCT sulle colonne del blocco
      for (size_t j = 0; j < w; ++j) {
        double* col = mat + by * width + (bx + j);
        if (idct1d(ctx, col, h, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}


DCT_API int dct2dblk(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  // Verifica che la dimensione del blocco non superi la dimensione massima supportata
  if (blk_size > ctx->max_n) return -1;

  // Calcola quanti blocchi completi possiamo fare in ogni dimensione
  size_t num_blocks_x = width / blk_size; // Solo blocchi completi in larghezza
  size_t num_blocks_y = height / blk_size; // Solo blocchi completi in altezza

  // Se non possiamo fare almeno un blocco completo, restituisci errore
  if (num_blocks_x == 0 || num_blocks_y == 0) return -1;

  // Elabora solo i blocchi completi
  for (size_t by = 0; by < num_blocks_y * blk_size; by += blk_size) {
    for (size_t bx = 0; bx < num_blocks_x * blk_size; bx += blk_size) {
      // DCT sulle righe del blocco
      for (size_t i = 0; i < blk_size; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (dct1d(ctx, row, blk_size, 1, row) < 0)
          return -1;
      }
      // DCT sulle colonne del blocco
      for (size_t j = 0; j < blk_size; ++j) {
        double* col = mat + by * width + (bx + j);
        if (dct1d(ctx, col, blk_size, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}

// iDCT 2D a blocchi completi (ignora blocchi parziali)
DCT_API int idct2dblk(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  // Verifica che la dimensione del blocco non superi la dimensione massima supportata
  if (blk_size > ctx->max_n) return -1;

  // Calcola quanti blocchi completi possiamo fare in ogni dimensione
  size_t num_blocks_x = width / blk_size; // Solo blocchi completi in larghezza
  size_t num_blocks_y = height / blk_size; // Solo blocchi completi in altezza

  // Se non possiamo fare almeno un blocco completo, restituisci errore
  if (num_blocks_x == 0 || num_blocks_y == 0) return -1;

  // Elabora solo i blocchi completi
  for (size_t by = 0; by < num_blocks_y * blk_size; by += blk_size) {
    for (size_t bx = 0; bx < num_blocks_x * blk_size; bx += blk_size) {
      // iDCT sulle righe del blocco
      for (size_t i = 0; i < blk_size; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (idct1d(ctx, row, blk_size, 1, row) < 0)
          return -1;
      }
      // iDCT sulle colonne del blocco
      for (size_t j = 0; j < blk_size; ++j) {
        double* col = mat + by * width + (bx + j);
        if (idct1d(ctx, col, blk_size, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}

#endif
