import numpy as np
from PIL import Image
from PySide6.QtCore import QObject, Signal, Slot, QThread

from .lib.DCT2Handler import Progress


class BackgroundWorker(QObject):
    """
    Background worker to handle long-running tasks without blocking the main thread.
    """
    sig_img_converted = Signal(np.ndarray)
    sig_error = Signal(str)
    sig_progress = Signal(Progress)

    background_worker_thread: QThread

    def __init__(self):
        super(BackgroundWorker, self).__init__()

    @Slot(str)
    def convert_img_to_array(self, image_file: str):
        """
        Convert a PIL image to a numpy array.
        """
        try:
            if image_file is not None:
                image_arr = np.array(Image.open(image_file))
                self.sig_img_converted.emit(image_arr)
            else:
                self.sig_error.emit("No image file provided.")
        except Exception as e:
            self.sig_error.emit("Error: " + str(e))
            self.sig_progress.emit(Progress.ERROR)

    def close(self):
        """
        Close the background worker thread.
        """
        self.background_worker_thread.quit()
        self.background_worker_thread.wait()
        self.deleteLater()
