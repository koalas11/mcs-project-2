from typing import Optional

import numpy as np
from PIL import Image
from PIL.ImageQt import QPixmap
from PySide6.QtCore import Qt, Slot, Signal
from PySide6.QtGui import QPainter, QPen
from PySide6.QtWidgets import QWidget, QGraphicsPixmapItem, QGraphicsScene, QVBoxLayout

from .LoadingCircle import LoadingCircle
from .Settings import Settings
from .lib.DCT2Handler import Progress
from .ui_generated.ui_ImagesWidget import Ui_ImagesWidget


def update_display(pixmap: QPixmap, item: QGraphicsPixmapItem, no_grid = False):
    """Update the displayed pixmap with or without grid"""
    if pixmap is None:
        return

    display_pixmap = pixmap.copy()
    painter = QPainter(display_pixmap)
    painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)
    # Create a copy of the original pixmap

    painter.drawPixmap(0, 0, display_pixmap)

    # Draw grid if enabled
    if Settings.grid_enabled and not no_grid:
        color = Settings.grid_color
        color.setAlpha(240)

        pen = QPen(color)

        # Calcola la dimensione dei blocchi
        block_width = display_pixmap.width() // Settings.block_size
        block_height = display_pixmap.height() // Settings.block_size
        x_size = block_width * Settings.block_size
        y_size = block_height * Settings.block_size

        pen.setWidth(display_pixmap.width() // 200)
        painter.setPen(pen)
        # Disegna linee orizzontali per blocchi completi
        for i in range(Settings.block_size + 1):
            y = i * block_height
            painter.drawLine(0, y, x_size, y)

        pen.setWidth(display_pixmap.height() // 200)
        painter.setPen(pen)
        # Disegna linee verticali per blocchi completi
        for i in range(Settings.block_size + 1):
            x = i * block_width
            painter.drawLine(x, 0, x, y_size)

    painter.end()

    item.setPixmap(display_pixmap)


class ImagesWidget(QWidget):

    raw_original_img_pixmap: Optional[QPixmap] = None
    raw_processed_img_pixmap: Optional[QPixmap] = None

    loadig_layout: Optional[QVBoxLayout] = None
    loading_circle: Optional[LoadingCircle] = None

    sig_fit_items = Signal()

    def __init__(self, parent=None):
        super(ImagesWidget, self).__init__(parent)
        
        self.ui = Ui_ImagesWidget()
        self.ui.setupUi(self)

        # Set up the graphics view
        self.ui.OriginalImg.setRenderHint(QPainter.RenderHint.Antialiasing, False)  # Enable antialiasing
        self.ui.OriginalImg.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)  # Smooth scaling
        scene = QGraphicsScene()
        self.ui.OriginalImg.setScene(scene)

        self.PixmapItem = QGraphicsPixmapItem()
        self.ui.OriginalImg.scene().addItem(self.PixmapItem)

        self.ui.ProcessedImg.setRenderHint(QPainter.RenderHint.Antialiasing, False)  # Enable antialiasing
        self.ui.ProcessedImg.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)  # Smooth scaling
        scene = QGraphicsScene()
        self.ui.ProcessedImg.setScene(scene)

        self.ProcessedPixmapItem = QGraphicsPixmapItem()
        self.ui.ProcessedImg.scene().addItem(self.ProcessedPixmapItem)

        self.ui.OriginalImg.hide()
        self.ui.ProcessedImg.hide()

        self.sig_fit_items.connect(self.fit_items, Qt.ConnectionType.QueuedConnection)

    @Slot(str)
    def load_image(self, image_array: np.ndarray):
        """Load an image from file path"""
        self.raw_original_img_pixmap = Image.fromarray(image_array).toqpixmap()
        if self.raw_original_img_pixmap:
            update_display(self.raw_original_img_pixmap, self.PixmapItem)
            self.ui.OriginalImg.show()
            self.sig_fit_items.emit()

    @Slot(np.ndarray)
    def load_processed_image(self, image_array: np.ndarray):
        """Load a processed image"""
        self.raw_processed_img_pixmap = Image.fromarray(image_array).toqpixmap()
        if self.raw_processed_img_pixmap:
            update_display(self.raw_processed_img_pixmap, self.ProcessedPixmapItem, no_grid=True)
            self.ui.ProcessedImg.show()
            self.sig_fit_items.emit()

    @Slot(Progress)
    def on_progress_update(self, progress: Progress):
        """Update the progress of the image processing"""
        if progress == Progress.STARTED:
            self.loading_circle = LoadingCircle(self)
            self.ui.ProcessedImg.hide()
            self.loadig_layout = QVBoxLayout()
            self.loadig_layout.addStretch()
            self.loadig_layout.addWidget(self.loading_circle, alignment=Qt.AlignmentFlag.AlignHCenter)
            self.loadig_layout.addStretch()

            self.ui.MainLayout.addLayout(self.loadig_layout)
            self.loading_circle.start()
        else:
            if self.loading_circle is None:
                return
            self.loading_circle.hide()
            self.loading_circle.stop()
            self.loadig_layout.removeWidget(self.loading_circle)
            self.loadig_layout.deleteLater()
            self.loading_circle.deleteLater()
            self.loading_circle = None

    @Slot()
    def on_grid_changed(self):
        update_display(self.raw_original_img_pixmap, self.PixmapItem)

    @Slot()
    def fit_items(self):
        """Fit the items in the view"""
        self.ui.OriginalImg.fitInView(self.PixmapItem, Qt.AspectRatioMode.KeepAspectRatio)
        self.ui.ProcessedImg.fitInView(self.ProcessedPixmapItem, Qt.AspectRatioMode.KeepAspectRatio)

    def resizeEvent(self, event):
        """Handle resize events"""
        super(ImagesWidget, self).resizeEvent(event)
        self.fit_items()
