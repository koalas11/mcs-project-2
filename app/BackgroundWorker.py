import numpy as np
from PIL import Image
from PySide6.QtCore import QObject, Signal, Slot, QThread


class BackgroundWorker(QObject):
    """
    Background worker to handle long-running tasks without blocking the main thread.
    """
    sig_img_converted = Signal(int, np.ndarray)

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
                self.sig_img_converted.emit(0, image_arr)
            else:
                self.sig_img_converted.emit(-1, "error")
        except Exception as e:
            self.sig_img_converted.emit(-1, str(e))

    def close(self):
        """
        Close the background worker thread.
        """
        self.background_worker_thread.quit()
        self.background_worker_thread.wait()
        self.deleteLater()
