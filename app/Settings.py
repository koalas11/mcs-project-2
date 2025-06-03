from PySide6.QtGui import QColor


class Settings:
    block_size: int = 1
    cut_off_threshold: int = 0

    grid_color: QColor = QColor.fromRgb(255, 120, 0)
    grid_enabled: bool = True
    use_scipy: bool = False
