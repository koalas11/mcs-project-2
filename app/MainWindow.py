from typing import Optional

from PySide6.QtCore import QThread, Qt
from PySide6.QtWidgets import QMainWindow

from .BackgroundWorker import BackgroundWorker
from .ImagesHandler import ImagesHandler
from .ImagesWidget import ImagesWidget
from .MenuWidget import MenuWidget
from .lib.DCT2Handler import DCT2Handler
from .ui_generated.ui_MainWindow import Ui_MainWindow


class MainWindow(QMainWindow):
    images_widget: ImagesWidget
    menu_widget: MenuWidget
    images_handler: ImagesHandler
    background_worker: BackgroundWorker
    dct2_handler: DCT2Handler

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.setWindowTitle("Main Window")
        self.menu_widget = MenuWidget(self)
        self.images_widget = ImagesWidget(self)
        self.images_handler = ImagesHandler(self)
        self.background_worker = BackgroundWorker()

        self.dct2_handler = DCT2Handler()

        if self.dct2_handler.lib is not None:
            self.dct2_handler.dct2_handler_thread = QThread()
            self.dct2_handler.dct2_handler_thread.setObjectName("DCT2HandlerThread")
            self.dct2_handler.moveToThread(self.dct2_handler.dct2_handler_thread)
            self.dct2_handler.dct2_handler_thread.start()

        self.background_worker.background_worker_thread = QThread()
        self.background_worker.moveToThread(self.background_worker.background_worker_thread)
        self.background_worker.background_worker_thread.setObjectName("BackgroundWorkerThread")
        self.background_worker.background_worker_thread.start()

        self.menu_widget.sig_image_selected.connect(self.background_worker.convert_img_to_array, Qt.ConnectionType.QueuedConnection)
        self.background_worker.sig_img_converted.connect(self.images_handler.load_image)

        self.images_handler.sig_update_original_img.connect(self.images_widget.load_image)
        self.images_handler.sig_update_processed_img.connect(self.images_widget.load_processed_image)
        self.dct2_handler.sig_img_processing_updates.connect(self.images_widget.on_progress_update)
        self.dct2_handler.sig_img_processing_updates.connect(self.menu_widget.on_progress_update)
        self.images_handler.sig_process_img.connect(self.dct2_handler.compress)

        self.menu_widget.sig_grid_changed.connect(self.images_widget.on_grid_changed)
        self.menu_widget.sig_apply_button_clicked.connect(self.images_handler.start_processing)

        self.dct2_handler.sig_img_processed.connect(self.images_handler.on_processed_image)

        self.ui.MainLayout.addWidget(self.menu_widget)
        self.ui.MainLayout.addWidget(self.images_widget)
        self.ui.MainLayout.setStretch(1, 3)

        QThread.currentThread().setObjectName("MainWindowThread")

    def closeEvent(self, event):
        self.dct2_handler.close()
        self.background_worker.close()
        super(MainWindow, self).closeEvent(event)