from PySide6.QtCore import Qt, Slot, QStandardPaths, Signal
from PySide6.QtGui import QColor
from PySide6.QtWidgets import QWidget, QMessageBox, QFileDialog, QColorDialog

from .lib.DCT2Handler import Progress
from .ui_generated.ui_MenuWidget import Ui_MenuWidget


class MenuWidget(QWidget):
    block_size: int = 1
    cut_off_threshold: int = 0

    grid_color: QColor = QColor.fromRgb(255, 0, 0)
    grid_enabled: bool = True


    sig_image_selected = Signal(str)
    sig_grid_changed = Signal()
    sig_apply_button_clicked = Signal()

    def __init__(self, parent=None):
        super(MenuWidget, self).__init__(parent)
        self.ui = Ui_MenuWidget()
        self.ui.setupUi(self)

        self.ui.GridSizeSpinBox.setValue(MenuWidget.block_size)
        self.ui.GridColorValue.setStyleSheet(f"background-color: {MenuWidget.grid_color.name()};")
        self.ui.CutOffThresholdSpinBox.setValue(MenuWidget.cut_off_threshold)

        self.ui.ImageBrowseButton.clicked.connect(self.on_browse_file_button_clicked)
        self.ui.ToggleGridButton.clicked.connect(self.on_grid_toggle)
        self.ui.GridSizeSpinBox.valueChanged.connect(self.on_grid_size_changed)
        self.ui.GridColorValue.clicked.connect(self.on_grid_color_button_clicked)
        self.ui.CutOffThresholdSpinBox.valueChanged.connect(self.on_cut_off_threshold_changed)
        self.ui.ApplyButton.clicked.connect(self.on_apply_button_clicked)

    @Slot()
    def on_browse_file_button_clicked(self):
        file_dialog = QFileDialog()
        file_dialog.setFileMode(QFileDialog.FileMode.ExistingFile)
        path = QStandardPaths.standardLocations(QStandardPaths.StandardLocation.HomeLocation.DesktopLocation)
        file=  QFileDialog.getOpenFileName(self, self.tr("Select Image"), path[0], "Image (*.bmp)")[0]
        if file == "":
            return
        self.on_image_path_changed(file)

    @Slot(str)
    def on_image_path_changed(self, file: str):
        self.ui.ImagePathLineEdit.setText(file)
        self.sig_image_selected.emit(file)

    @Slot()
    def on_grid_toggle(self):
        MenuWidget.grid_enabled = not MenuWidget.grid_enabled
        if MenuWidget.grid_enabled:
            self.ui.ToggleGridButton.setText("Disable Grid")
        else:
            self.ui.ToggleGridButton.setText("Enable Grid")
        self.sig_grid_changed.emit()

    @Slot(int)
    def on_grid_size_changed(self, size: int):
        MenuWidget.block_size = size
        self.sig_grid_changed.emit()

    @Slot()
    def on_grid_color_button_clicked(self):
        color = QColorDialog.getColor(Qt.GlobalColor.red, self, self.tr("Select Grid Color"))
        if color.isValid():
            MenuWidget.grid_color = color
            self.sig_grid_changed.emit()
            self.ui.GridColorValue.setStyleSheet(f"background-color: {color.name()};")

    @Slot(int)
    def on_cut_off_threshold_changed(self, value: int):
        MenuWidget.cut_off_threshold = value

    @Slot()
    def on_apply_button_clicked(self):
        self.ui.ApplyButton.setEnabled(False)
        self.sig_apply_button_clicked.emit()

    @Slot(Progress)
    def on_progress_update(self, progress: Progress):
        if progress == Progress.STARTED:
            self.ui.ApplyButton.setText("Processing...")
        elif progress == Progress.FINISHED:
            self.ui.ApplyButton.setText("Apply")
            self.ui.ApplyButton.setEnabled(True)
        elif progress == Progress.ERROR:
            self.ui.ApplyButton.setText("Apply")
            self.ui.ApplyButton.setEnabled(True)
            QMessageBox.critical(self, "Error", "An error occurred during processing.")
