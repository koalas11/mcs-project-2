import ctypes
import sys
from enum import IntEnum

import numpy as np
from PySide6.QtCore import QObject, QThread, Slot, Signal
from scipy.fftpack import dctn, idctn

from app.Settings import Settings
from .DCT2Library import DCT2Library, DoublePtr


class Progress(IntEnum):
    STARTED = 0
    FINISHED = 1
    ERROR = -1


def apply_cutoff_to_blocks(
        matrix: np.ndarray,
    block_size: int,
    cut_off_threshold: int,
):
    for r in range(matrix.shape[0] // block_size):
        for c in range(matrix.shape[1] // block_size):
            block = matrix[r * block_size:(r + 1) * block_size, c * block_size:(c + 1) * block_size]

            for k in range(block_size):
                for l in range(block_size):
                    if k + l >= cut_off_threshold:
                        block[k, l] = 0


class DCT2Handler(QObject):
    lib: DCT2Library

    dct2_handler_thread: QThread
    lib_load_error: str

    sig_img_processing_updates = Signal(Progress)
    sig_img_processed = Signal(np.ndarray)

    def __init__(self):
        try:
            DCT2Handler.lib = DCT2Library()
        except Exception as e:
            DCT2Handler.lib = None
            DCT2Handler.lib_load_error = "Could not load the library.\n" + str(e)
        super(DCT2Handler, self).__init__()

    def dct_init(self, context: DCT2Library.DCTContextPtr):
        self.lib.dct_init(context)

    def dct_shutdown(self, context: DCT2Library.DCTContextPtr):
        self.lib.dct_shutdown(context)

    @Slot(np.ndarray, int, int, Signal)
    def compress(
        self,
        matrix_or: np.ndarray,
        block_size : int,
        cut_off_threshold : int,
    ):
        assert matrix_or.flags['C_CONTIGUOUS']
        assert matrix_or.ndim in {2, 3}

        self.sig_img_processing_updates.emit(Progress.STARTED)

        old_type = matrix_or.dtype
        matrix = np.ascontiguousarray(matrix_or.astype(dtype=np.float64))

        if matrix.ndim == 3:
            for i in range(matrix.shape[2]):
                tmp_matrix = np.ascontiguousarray(matrix[:, :, i])
                self.dct2d(tmp_matrix, block_size, cut_off_threshold)
                matrix[:, :, i] = tmp_matrix
        else:
            self.dct2d(matrix, block_size, cut_off_threshold)

        matrix_new = matrix.round().clip(0, 255).astype(old_type)
        self.sig_img_processed.emit(matrix_new)
        self.sig_img_processing_updates.emit(Progress.FINISHED)


    def dct2d(
        self,
        matrix: np.ndarray,
        block_size : int,
        cut_off_threshold : int,
    ):
        assert matrix.flags['C_CONTIGUOUS']
        assert len(matrix.shape) == 2

        block_size_t = ctypes.c_size_t(block_size)

        ctx = self.lib.dct_context_alloc(block_size_t)
        result = self.lib.dct_init(ctx)

        sys.stderr.flush()

        if result != 0:
            self.sig_img_processing_updates.emit(Progress.ERROR)
            return

        height = ctypes.c_size_t(matrix.shape[0])
        width = ctypes.c_size_t(matrix.shape[1])

        matrix_ptr = matrix.ctypes.data_as(DoublePtr)

        if Settings.use_scipy:
            for r in range(matrix.shape[0] // block_size):
                for c in range(matrix.shape[1] // block_size):
                    block: np.typing.NDArray = dctn(matrix[r * block_size: (r + 1) * block_size, c * block_size: (c + 1) * block_size], norm='ortho')

                    for k in range(block_size):
                        for l in range(block_size):
                            if k + l >= cut_off_threshold:
                                block[k, l] = 0

                    matrix[r * block_size: (r + 1) * block_size, c * block_size: (c + 1) * block_size] = idctn(block, norm='ortho')
        else:
            result = self.lib.dct2dblk(ctx, matrix_ptr, width, height, block_size_t)  # Applica DCT2D

            if result != 0:
                self.sig_img_processing_updates.emit(Progress.ERROR)
                return

            apply_cutoff_to_blocks(matrix, block_size, cut_off_threshold)

            result = self.lib.idct2dblk(ctx, matrix_ptr, width, height, block_size_t)

            if result != 0:
                self.sig_img_processing_updates.emit(Progress.ERROR)
                return

        self.lib.dct_shutdown(ctx)
        self.lib.dct_context_free(ctx)

    def close(self):
        if self.lib is not None:
            self.dct2_handler_thread.quit()
            self.dct2_handler_thread.wait()
            self.deleteLater()
