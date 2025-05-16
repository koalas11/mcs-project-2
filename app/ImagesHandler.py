from typing import Optional

import numpy as np
from PIL import Image
from PySide6.QtCore import QObject, Signal, Slot
from PySide6.QtWidgets import QMessageBox

from app.MenuWidget import MenuWidget
from app.lib.DCT2Handler import Progress


class ImagesHandler(QObject):

    original_img_arr: Optional[np.ndarray] = None
    processed_img: Optional[np.ndarray] = None
    tmp_img_arr: Optional[np.ndarray] = None

    sig_update_original_img = Signal(np.ndarray)
    sig_update_processed_img = Signal(np.ndarray)

    sig_process_img = Signal(np.ndarray, int, int, Signal)
    sig_process_img_update = Signal(Progress)

    def __init__(self, parent=None):
        super(ImagesHandler, self).__init__(parent)

    @Slot(int, (np.ndarray), (str))
    def load_image(self, error_code: int, image: np.ndarray | str):
        """Load an image into the handler."""
        if error_code != 0:
            raise RuntimeError(f"Error code: {error_code}")
        if image is not None and isinstance(image, np.ndarray):
            self.original_img_arr = image

            self.tmp_img_arr = None
            self.sig_update_original_img.emit(self.original_img_arr)
        else:
            raise ValueError("Invalid image data provided.")

    @Slot()
    def start_processing(self):
        """Process the loaded image using the provided function."""
        if self.original_img_arr is not None:
            self.tmp_img_arr = np.copy(self.original_img_arr)
            self.sig_process_img.emit(self.tmp_img_arr, MenuWidget.block_size, MenuWidget.cut_off_threshold, self.sig_process_img_update)
        else:
            raise ValueError("No image loaded to process.")

    @Slot(np.ndarray)
    def on_processed_image(self, processed_image: np.ndarray):
        """Handle the processed image."""
        if processed_image is not None:
            self.processed_img = processed_image
            self.sig_update_processed_img.emit(self.processed_img)
        else:
            raise ValueError("No processed image available.")