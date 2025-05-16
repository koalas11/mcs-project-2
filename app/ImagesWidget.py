from typing import Optional

from PIL.ImageQt import QPixmap
from PySide6.QtCore import Qt, Slot, QLineF, QEvent, Signal
from PySide6.QtGui import QPainter, QPen, QColor, QImage
from PySide6.QtWidgets import QWidget, QGraphicsPixmapItem, QGraphicsScene, QGraphicsView, QVBoxLayout
from PIL import Image
import numpy as np

from .LoadingCircle import LoadingCircle
from .MenuWidget import MenuWidget
from .lib.DCT2Handler import Progress
from .ui_generated.ui_ImagesWidget import Ui_ImagesWidget


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
            self.update_display(self.raw_original_img_pixmap, self.PixmapItem)
            self.ui.OriginalImg.show()
            self.sig_fit_items.emit()

    @Slot(np.ndarray)
    def load_processed_image(self, image_array: np.ndarray):
        """Load a processed image"""
        self.raw_processed_img_pixmap = Image.fromarray(image_array).toqpixmap()
        if self.raw_processed_img_pixmap:
            self.update_display(self.raw_processed_img_pixmap, self.ProcessedPixmapItem, no_grid=True)
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
            self.loading_circle.hide()
            self.loading_circle.stop()
            self.loadig_layout.removeWidget(self.loading_circle)
            self.loadig_layout.deleteLater()
            self.loading_circle.deleteLater()
            self.loading_circle = None

    @Slot()
    def on_grid_changed(self):
        self.update_display(self.raw_original_img_pixmap, self.PixmapItem, self.ui.OriginalImg)

    @Slot()
    def fit_items(self):
        """Fit the items in the view"""
        self.ui.OriginalImg.fitInView(self.PixmapItem, Qt.AspectRatioMode.KeepAspectRatio)
        self.ui.ProcessedImg.fitInView(self.ProcessedPixmapItem, Qt.AspectRatioMode.KeepAspectRatio)

    def resizeEvent(self, event):
        """Handle resize events"""
        super(ImagesWidget, self).resizeEvent(event)
        self.fit_items()

    def update_display(self, pixmap: QPixmap, item: QGraphicsPixmapItem, no_grid = False):
        """Update the displayed pixmap with or without grid"""
        if pixmap is None:
            return

        display_pixmap = pixmap.copy()
        painter = QPainter(display_pixmap)
        painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)
        # Create a copy of the original pixmap

        painter.drawPixmap(0, 0, display_pixmap)

        # Draw grid if enabled
        if MenuWidget.grid_enabled and not no_grid:
            pen = QPen(MenuWidget.grid_color)
            pen.setWidth(1)
            painter.setPen(pen)

            # Calcola la dimensione dei blocchi
            block_width = display_pixmap.width() // MenuWidget.block_size
            block_height = display_pixmap.height() // MenuWidget.block_size

            # Disegna linee orizzontali per blocchi completi
            for i in range(0, MenuWidget.block_size + 1):
                y = i * block_height
                painter.drawLine(0, y, block_width * MenuWidget.block_size, y)

            # Disegna linee verticali per blocchi completi
            for i in range(0, MenuWidget.block_size + 1):
                x = i * block_width
                painter.drawLine(x, 0, x, block_height * MenuWidget.block_size)

        painter.end()

        item.setPixmap(display_pixmap)
