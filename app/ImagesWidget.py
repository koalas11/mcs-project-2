from typing import Optional

from PySide6.QtCore import Qt, Slot, QLineF
from PySide6.QtGui import QPixmap, QPainter, QPen, QColor, QImage
from PySide6.QtWidgets import QWidget, QGraphicsPixmapItem, QGraphicsScene

from .ui_generated.ui_ImagesWidget import Ui_ImagesWidget


class ImagesWidget(QWidget):

    def __init__(self, parent=None):
        super(ImagesWidget, self).__init__(parent)
        
        self.ui = Ui_ImagesWidget()
        self.ui.setupUi(self)
        
        self.raw_pixmap: QPixmap = QPixmap()
        self.display_pixmap: QPixmap = QPixmap()
        self.grid_size: int = 5
        self.grid_color: QColor = QColor.fromRgb(255, 0, 0)
        self.grid_enabled: bool = True

        # Set up the graphics view
        self.ui.OriginalImg.setRenderHint(QPainter.RenderHint.Antialiasing, False)  # Enable antialiasing
        self.ui.OriginalImg.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)  # Smooth scaling
        scene = QGraphicsScene()
        self.ui.OriginalImg.setScene(scene)

        self.PixmapItem = QGraphicsPixmapItem()
        self.ui.OriginalImg.scene().addItem(self.PixmapItem)

    @Slot(str)
    def load_image(self, file_path: str):
        """Load an image from file path"""
        if self.raw_pixmap.load(file_path):
            self.update_display()

    @Slot(int)
    def set_grid_size(self, size: int):
        """Set grid cell size in pixels"""
        if size > 0:
            self.grid_size = size
            self.update_display()

    @Slot(QColor)
    def set_grid_color(self, color: QColor):
        """Set grid line color"""
        self.grid_color = color
        self.update_display()

    @Slot()
    def toggle_grid(self, enabled=None):
        """Toggle grid visibility"""
        if enabled is not None:
            self.grid_enabled = enabled
        else:
            self.grid_enabled = not self.grid_enabled
        self.update_display()

    def update_display(self):
        """Update the displayed pixmap with or without grid"""
        if self.raw_pixmap.isNull():
            return

        self.display_pixmap = self.raw_pixmap.copy()
        painter = QPainter(self.display_pixmap)
        painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)
        # Create a copy of the original pixmap

        painter.drawPixmap(0, 0, self.display_pixmap)

        # Draw grid if enabled
        if self.grid_enabled:

            pen = QPen(self.grid_color)
            pen.setWidth(1)
            painter.setPen(pen)
            
            # Draw horizontal lines
            line = QLineF(0, 0, 0, 0)

            size = int(self.display_pixmap.height() / self.grid_size)
            for y in range(size, self.display_pixmap.height(), size):
                line.setLine(0, y, self.display_pixmap.width(), y)
                painter.drawLine(line)

            size = int(self.display_pixmap.width() / self.grid_size)
            # Draw vertical lines
            for x in range(size, self.display_pixmap.width(), size):
                line.setLine(x, 0, x, self.display_pixmap.height())
                painter.drawLine(line)

        painter.end()

        self.PixmapItem.setPixmap(self.display_pixmap)
        self.ui.OriginalImg.fitInView(self.PixmapItem)

    def resizeEvent(self, event):
        """Handle resize events"""
        super(ImagesWidget, self).resizeEvent(event)
        self.ui.OriginalImg.fitInView(self.PixmapItem)
