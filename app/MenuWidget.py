from PIL import Image
from PySide6.QtCore import Qt, Slot, QStandardPaths, Signal
from PySide6.QtWidgets import QWidget, QMessageBox, QFileDialog, QColorDialog

from .ImagesHandler import ImagesHandler
from .Settings import Settings
from .lib.DCT2Handler import Progress
from .ui_generated.ui_MenuWidget import Ui_MenuWidget


class MenuWidget(QWidget):

    sig_image_selected = Signal(str)
    sig_grid_changed = Signal()
    sig_apply_button_clicked = Signal()

    img_size_x: int = 0
    img_size_y: int = 0

    def __init__(self, parent=None):
        super(MenuWidget, self).__init__(parent)
        self.ui = Ui_MenuWidget()
        self.ui.setupUi(self)

        self.ui.GridSizeSpinBox.setValue(Settings.block_size)
        self.ui.GridColorValue.setStyleSheet(f"background-color: {Settings.grid_color.name()};")
        self.ui.CutOffThresholdSpinBox.setValue(Settings.cut_off_threshold)

        self.ui.ImageBrowseButton.clicked.connect(self.on_browse_file_button_clicked)
        self.ui.ToggleGridButton.clicked.connect(self.on_grid_toggle)
        self.ui.GridSizeSpinBox.valueChanged.connect(self.on_grid_size_changed)
        self.ui.GridColorValue.clicked.connect(self.on_grid_color_button_clicked)
        self.ui.CutOffThresholdSpinBox.valueChanged.connect(self.on_cut_off_threshold_changed)
        self.ui.ApplyButton.clicked.connect(self.on_apply_button_clicked)
        self.ui.SaveProcessedImageButton.clicked.connect(self.save_processed_image)
        self.ui.ScipyImplCheckBox.checkStateChanged.connect(self.on_scipy_impl_changed)

    @Slot(int, int)
    def on_image_loaded(self, size_x: int, size_y: int):
        """Update the UI when an image is loaded."""
        self.ui.GridSizeSpinBox.setEnabled(True)
        self.ui.CutOffThresholdSpinBox.setEnabled(True)
        size = min(size_x, size_y)
        self.img_size_x = size_x
        self.img_size_y = size_y
        self.ui.GridSizeSpinBox.setRange(2, size)
        self.ui.GridSizeSpinBox.setValue(int(size * 0.1))
        self.ui.ApplyButton.setEnabled(True)

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
        Settings.grid_enabled = not Settings.grid_enabled
        if Settings.grid_enabled:
            self.ui.ToggleGridButton.setText("Disable Grid")
        else:
            self.ui.ToggleGridButton.setText("Enable Grid")
        self.sig_grid_changed.emit()

    @Slot(int)
    def on_grid_size_changed(self, size: int):
        Settings.block_size = size
        self.ui.NumBlocksValue.setText(str((self.img_size_x // size) * (self.img_size_y // size)))
        self.ui.CutOffThresholdSpinBox.setRange(0, size * 2 - 2)
        self.sig_grid_changed.emit()

    @Slot()
    def on_grid_color_button_clicked(self):
        color = QColorDialog.getColor(Qt.GlobalColor.red, self, self.tr("Select Grid Color"))
        if color.isValid():
            Settings.grid_color = color
            self.sig_grid_changed.emit()
            self.ui.GridColorValue.setStyleSheet(f"background-color: {color.name()};")

    @Slot(int)
    def on_cut_off_threshold_changed(self, value: int):
        Settings.cut_off_threshold = value

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
            if self.ui.ApplyButton.text() == "Processing...":
                QMessageBox.critical(self, "Error", "An error occurred during processing.")

    @Slot()
    def save_processed_image(self):
        """Save the processed image to a file."""
        if ImagesHandler.processed_img is not None:
            try:
                img = Image.fromarray(ImagesHandler.processed_img)
                path = QStandardPaths.standardLocations(QStandardPaths.StandardLocation.HomeLocation.DesktopLocation)
                fileName, _ = QFileDialog.getSaveFileName(self, "Save Image", path[0], "PNG Images (*.png);;BMP Images (*.bmp)")
                
                if fileName is None or fileName == "":
                    return
                
                img.save(fileName)
            except Exception as e:
                QMessageBox.critical(self, "Error", f"Failed to save image: {str(e)}")
        else:
            QMessageBox.warning(self, "Warning", "No processed image to save.")

    @Slot(Qt.CheckState)
    def on_scipy_impl_changed(self, state: Qt.CheckState):
        Settings.use_scipy = state == Qt.CheckState.Checked
