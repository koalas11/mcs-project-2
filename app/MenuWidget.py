from typing import Optional

from PySide6.QtCore import QSize, Qt, Slot, QThread, QStandardPaths, Signal
from PySide6.QtGui import QIcon, QColor
from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QSizePolicy, QMessageBox, QFileDialog, QColorDialog

from .ui_generated.ui_MenuWidget import Ui_MenuWidget

class MenuWidget(QWidget):

    sig_image_selected = Signal(str)
    sig_grid_toggle = Signal()
    sig_grid_size_changed = Signal(int)
    sig_grid_color_changed = Signal(QColor)

    def __init__(self, parent=None):
        super(MenuWidget, self).__init__(parent)
        
        self.ui = Ui_MenuWidget()
        self.ui.setupUi(self)

        self.ui.ImageBrowseButton.clicked.connect(self.on_browse_file_button_clicked)
        self.ui.ToggleGridButton.clicked.connect(self.sig_grid_toggle.emit)
        self.ui.GridSizeSpinBox.valueChanged.connect(self.sig_grid_size_changed.emit)
        self.ui.GridColorBrowseButton.clicked.connect(self.on_grid_color_button_clicked)

    def on_browse_file_button_clicked(self):
        file_dialog = QFileDialog()
        file_dialog.setFileMode(QFileDialog.FileMode.ExistingFile)
        path = QStandardPaths.standardLocations(QStandardPaths.StandardLocation.HomeLocation.DesktopLocation)
        file=  QFileDialog.getOpenFileName(self, self.tr("Select Image"), path[0], "Image (*.bmp)")[0]
        if file == "":
            return
        self.on_image_path_changed(file)

    def on_grid_color_button_clicked(self):
        color = QColorDialog.getColor(Qt.GlobalColor.red, self, self.tr("Select Grid Color"))
        if color.isValid():
            self.sig_grid_color_changed.emit(color)
            self.ui.GridColorValueLabel.setStyleSheet(f"background-color: {color.name()};")
        else:
            QMessageBox.warning(self, self.tr("Invalid Color"), self.tr("Please select a valid color."))

    def on_image_path_changed(self, file):
        self.ui.ImagePathLineEdit.setText(file)
        self.sig_image_selected.emit(file)