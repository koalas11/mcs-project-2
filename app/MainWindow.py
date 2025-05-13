from typing import Optional

from PySide6.QtCore import QSize, Qt, Slot, QThread
from PySide6.QtGui import QIcon
from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QSizePolicy, QMessageBox

from .ImagesWidget import ImagesWidget
from .MenuWidget import MenuWidget
from .ui_generated.ui_MainWindow import Ui_MainWindow


class MainWindow(QMainWindow):
    images_widget: Optional[ImagesWidget] = None
    menu_widget: Optional[MenuWidget] = None

    def __init__(self):
        super(MainWindow, self).__init__()

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.setWindowTitle("Main Window")
        self.menu_widget = MenuWidget(self)
        self.images_widget = ImagesWidget(self)

        self.menu_widget.sig_image_selected.connect(self.images_widget.load_image)
        self.menu_widget.sig_grid_toggle.connect(self.images_widget.toggle_grid)
        self.menu_widget.sig_grid_size_changed.connect(self.images_widget.set_grid_size)
        self.menu_widget.sig_grid_color_changed.connect(self.images_widget.set_grid_color)

        self.ui.MainLayout.addWidget(self.menu_widget)
        self.ui.MainLayout.addWidget(self.images_widget)
        self.ui.MainLayout.setStretch(1, 3)

        QThread.currentThread().setObjectName("MainWindowThread")

