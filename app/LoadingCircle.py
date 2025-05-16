from typing import Optional

from PySide6.QtCore import Qt, QRectF, QTimer
from PySide6.QtGui import QPainter, QPainterPath, QPen, QColor
from PySide6.QtWidgets import QWidget


class LoadingCircle(QWidget):

    def __init__(self, parent=None):
        super(LoadingCircle, self).__init__(parent)
        self.angle = 0
        self.timer: Optional[QTimer] = None
        self.setMinimumSize(208, 208)

    def start(self):
        if self.timer:
            self.timer.stop()
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_angle)
        self.timer.start(16)

    def stop(self):
        if self.timer:
            self.timer.stop()
            self.timer = None
        self.angle = 0

    def update_angle(self):
        self.angle = (self.angle + 5) % 360
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(self.rect(), Qt.GlobalColor.white)
        painter.translate(4, 4)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)

        path = QPainterPath()
        path.moveTo(100, 0)
        path.arcTo(QRectF(0, 0, 200, 200), 90 - self.angle, -90)

        pen = QPen()
        pen.setCapStyle(Qt.PenCapStyle.FlatCap)
        pen.setColor(QColor("#30b7e0"))
        pen.setWidth(8)
        painter.strokePath(path, pen)