from typing import Optional

import numpy as np
from PySide6.QtCore import QObject, Signal, Slot

from .Settings import Settings
from .lib.DCT2Handler import Progress


class ImagesHandler(QObject):

    original_img_arr: Optional[np.ndarray] = None
    processed_img: Optional[np.ndarray] = None

    sig_update_original_img = Signal(np.ndarray)
    sig_update_processed_img = Signal(np.ndarray)
    sig_original_img_loaded = Signal(int, int)
    sig_error = Signal(str)
    sig_progress = Signal(Progress)

    sig_process_img = Signal(np.ndarray, int, int)

    def __init__(self, parent=None):
        super(ImagesHandler, self).__init__(parent)
        self.tmp_img_arr: Optional[np.ndarray] = None

    @Slot(np.ndarray)
    def load_image(self, image: np.ndarray):
        """Load an image into the handler."""
        if image is not None and isinstance(image, np.ndarray):
            ImagesHandler.original_img_arr = image

            self.tmp_img_arr = None
            self.sig_update_original_img.emit(ImagesHandler.original_img_arr)
            self.sig_original_img_loaded.emit(image.shape[1], image.shape[0])
        else:
            self.sig_error.emit("Invalid image data provided.")
            self.sig_progress.emit(Progress.ERROR)

    @Slot()
    def start_processing(self):
        """Process the loaded image using the provided function."""
        if ImagesHandler.original_img_arr is not None:
            self.tmp_img_arr = np.copy(ImagesHandler.original_img_arr)
            self.sig_process_img.emit(self.tmp_img_arr, Settings.block_size, Settings.cut_off_threshold)
        else:
            self.sig_error.emit("No image loaded for processing.")
            self.sig_progress.emit(Progress.ERROR)

    @Slot(np.ndarray)
    def on_processed_image(self, processed_image: np.ndarray):
        """Handle the processed image."""
        if processed_image is not None:
            ImagesHandler.processed_img = processed_image
            self.sig_update_processed_img.emit(ImagesHandler.processed_img)
        else:
            self.sig_error.emit("Processed image is None.")
            self.sig_progress.emit(Progress.ERROR)
