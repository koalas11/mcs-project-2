import ctypes
from enum import IntEnum

import numpy as np
from PySide6.QtCore import QObject, QThread, Slot, Signal
from scipy.fftpack import dctn, idctn


from .DCT2Library import DCT2Library, DoublePtr


class Progress(IntEnum):
    STARTED = 0
    FINISHED = 1
    ERROR = -1


type ToggleSignal = Signal(Progress)


class DCT2Handler(QObject):
    lib: DCT2Library

    dct2_handler_thread: QThread
    lib_load_error: str

    sig_img_processed = Signal(np.ndarray)

    def __init__(self):
        try:
            DCT2Handler.lib = DCT2Library()
        except Exception as e:
            DCT2Handler.lib = None
            DCT2Handler.lib_load_error = "Could not load the library.\n" + str(e)
        super(DCT2Handler, self).__init__()

    @Slot(DCT2Library.DCTContextPtr)
    def dct_init(
        self,
        context: DCT2Library.DCTContextPtr
    ):
        print("DCT2Handler: dct_init")
        print("Current thread:", QThread.currentThread())
        self.lib.dct_init(context)

    @Slot(DCT2Library.DCTContextPtr)
    def dct_shutdown(self, context: DCT2Library.DCTContextPtr):
        print("DCT2Handler: dct_shutdown")
        print("Current thread:", QThread.currentThread())
        self.lib.dct_shutdown(context)

    @Slot(np.ndarray, int, int, Signal)
    def dct(
        self,
        matrix_or: np.ndarray,
        block_size : int,
        cut_off_threshold : int,
        sig_toggle: ToggleSignal,
    ):
        print("currentThread", QThread.currentThread())
        assert matrix_or.flags['C_CONTIGUOUS']
        assert len(matrix_or.shape) <= 3

        sig_toggle.emit(Progress.STARTED)

        old_m = matrix_or.copy()
        old_type = matrix_or.dtype
        if len(matrix_or.shape) == 3:
            matrix = np.ascontiguousarray(matrix_or[:, :, 0])
        else:
            matrix = matrix_or

        matrix = np.ascontiguousarray(matrix.astype(dtype=np.float64))

        ctx = self.lib.dct_context_alloc(ctypes.c_size_t(block_size))
        block_size_t = ctypes.c_size_t(block_size)

        width = ctypes.c_size_t(matrix.shape[1])
        height = ctypes.c_size_t(matrix.shape[0])

        matrix_ptr = matrix.ctypes.data_as(DoublePtr)

        result = self.lib.dct2dblk(ctx, matrix_ptr, width, height, block_size_t)  # Applica DCT2D

        if result != 0:
            sig_toggle.emit(Progress.ERROR)
            return

        for r in range(matrix.shape[0] // block_size):
            for c in range(matrix.shape[1] // block_size):
                block = matrix[r * block_size:(r + 1) * block_size, c * block_size:(c + 1) * block_size]

                for k in range(block_size):
                    for l in range(block_size):
                        if k + l >= cut_off_threshold:
                            block[k, l] = 0

        result = self.lib.idct2dblk(ctx, matrix_ptr, width, height, block_size_t)

        if result != 0:
            sig_toggle.emit(Progress.ERROR)
            return

        matrix_new = matrix.round().clip(0, 255).astype(dtype=old_type)
        self.sig_img_processed.emit(matrix_new)
        sig_toggle.emit(Progress.FINISHED)

        return

        for r in range(matrix.shape[0] // block_size):
            for c in range(matrix.shape[1] // block_size):
                block = dctn(matrix[r*block_size : (r+1)*block_size, c*block_size : (c+1)*block_size], norm='ortho')

                for k in range(block_size):
                    for l in range(block_size):
                        if k + l >= cut_off_threshold:
                            block[k,l] = 0

                matrix[r*block_size : (r+1)*block_size, c*block_size : (c+1)*block_size] = idctn(block, norm='ortho')

        matrix_new = matrix.clip(0, 255).round().astype(np.uint8)
        self.sig_img_processed.emit(matrix_new)
        sig_toggle.emit(Progress.FINISHED)


    @Slot(np.ndarray, int, int, Signal)
    def dctGAU(
        self,
        matrix: np.ndarray,
        block_size : int,
        cut_off_threshold : int,
        sig_toggle: ToggleSignal,
    ):
        assert matrix.flags['C_CONTIGUOUS']
        assert len(matrix.shape) in (2, 3)

        sig_toggle.emit(Progress.STARTED)

        old_type = matrix.dtype
        if len(matrix.shape) == 3:
            matrix = matrix[:, :, 0]
        matrix = np.ascontiguousarray(matrix.astype(dtype=np.float64))

        for r in range(matrix.shape[0] // block_size):
            for c in range(matrix.shape[1] // block_size):
                block = matrix[r * block_size:(r + 1) * block_size, c * block_size:(c + 1) * block_size]  # Estrai il blocco

                width = ctypes.c_size_t(block_size)
                height = ctypes.c_size_t(1)

                for row in range(block_size):
                    temp = np.zeros(block_size)  # Array temporaneo per la DCT1D
                    temp_ptr = temp.ctypes.data_as(DoublePtr)
                    block_row_ptr = block[row].ctypes.data_as(DoublePtr)
                    self.lib.dct1d(block_row_ptr, width, height, temp_ptr)
                    for i in range(block_size):
                        block[row, i] = temp[i]

                width = ctypes.c_size_t(block_size)
                height = ctypes.c_size_t(block_size)

                for col in range(block_size):
                    temp = np.zeros(block_size)  # Array temporaneo per la DCT1D
                    temp_ptr = temp.ctypes.data_as(DoublePtr)
                    block_row_ptr = block[:, col].ctypes.data_as(DoublePtr)
                    self.lib.dct1d(block_row_ptr, width, height, temp_ptr)
                    for i in range(block_size):
                        block[i, col] = temp[i]

                for k in range(block_size):
                    for l in range(block_size):
                        if k + l >= cut_off_threshold:
                            block[k, l] = 0

                width = ctypes.c_size_t(block_size)
                height = ctypes.c_size_t(1)

                for row in range(block_size):
                    temp = np.zeros(block_size)  # Array temporaneo per la DCT1D
                    temp_ptr = temp.ctypes.data_as(DoublePtr)
                    block_row_ptr = block[row].ctypes.data_as(DoublePtr)
                    self.lib.idct1d(block_row_ptr, width, height, temp_ptr)
                    for i in range(block_size):
                        block[row, i] = temp[i]

                width = ctypes.c_size_t(block_size)
                height = ctypes.c_size_t(block_size)

                for col in range(block_size):
                    temp = np.zeros(block_size)  # Array temporaneo per la DCT1D
                    temp_ptr = temp.ctypes.data_as(DoublePtr)
                    block_row_ptr = block[:, col].ctypes.data_as(DoublePtr)
                    self.lib.idct1d(block_row_ptr, width, height, temp_ptr)
                    for i in range(block_size):
                        block[i, col] = temp[i]

                continue

        matrix = matrix.clip(0, 255).round().astype(dtype=old_type)
        self.sig_img_processed.emit(matrix)
        sig_toggle.emit(Progress.FINISHED)

    def close(self):
        if self.lib is not None:
            self.dct2_handler_thread.quit()
            self.dct2_handler_thread.wait()
            self.deleteLater()
