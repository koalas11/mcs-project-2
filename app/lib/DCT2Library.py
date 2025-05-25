import ctypes
import os
import platform
from types import NoneType
from typing import Callable, TypeAlias


class DCTContext(ctypes.Structure):
    _fields_ = [
        ("max_n", ctypes.c_int),
        ("scratch", ctypes.POINTER(ctypes.c_double)),
    ]


DCTContextPtr = ctypes.POINTER(DCTContext)
DoublePtr = ctypes.POINTER(ctypes.c_double)

class DCT2Library(ctypes.CDLL):
    BOOL = ctypes.c_bool
    INT = ctypes.c_int
    UINT = ctypes.c_uint
    SIZE_T = ctypes.c_size_t
    USHORT = ctypes.c_ushort
    DOUBLE = ctypes.c_double
    DCTContextPtr = DCTContextPtr
    DoublePtr = DoublePtr

    dct_context_alloc: Callable[[SIZE_T], DCTContextPtr]
    dct_context_free: Callable[[DCTContextPtr], None]

    # Main function to perform DCT
    dct_init: Callable[[DCTContextPtr], INT]
    dct_shutdown: Callable[[DCTContextPtr], INT]

    # DCT functions
    dct2d: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T], INT]
    idct2d: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T], INT]

    dct1d: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, DoublePtr], INT]
    idct1d: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, DoublePtr], INT]

    dct2dblk: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, SIZE_T], INT]
    idct2dblk: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, SIZE_T], INT]

    # dct2dblkrounded: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, SIZE_T], INT]
    # idct2dblkrounded: Callable[[DCTContextPtr, DoublePtr, SIZE_T, SIZE_T, SIZE_T], INT]


    def __init__(self):

        plat = platform.system()
        library_name = "libdct2.so"
        if plat == "Windows":
            library_name = "dct2.dll"
        elif plat == "Darwin":
            library_name = "libdct2.dylib"

        super().__init__(os.path.join(os.getcwd(), "target", library_name))
        method_name: str
        call_method: Callable
        for method_name, call_method in vars(type(self))["__annotations__"].items():
            if hasattr(call_method, "__args__"):
                arg_types = call_method.__args__[:-1]  # get the argument types
                res_type = call_method.__args__[-1]  # get the return type
                if res_type is NoneType:
                    res_type = None
                getattr(self, method_name).argtypes = arg_types
                getattr(self, method_name).restype = res_type
