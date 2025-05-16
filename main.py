import sys

from PySide6.QtCore import QSettings
from PySide6.QtWidgets import QApplication

from app.MainWindow import MainWindow

# Main entry point of the application.
if __name__ == "__main__":
    app = QApplication(sys.argv)

    app.setOrganizationName("SanvitoPellegrino")
    app.setApplicationName("DCTApp")
    QSettings.setDefaultFormat(QSettings.Format.IniFormat)

    main_window = MainWindow()

    main_window.show()

    # Start the application event loop.
    sys.exit(app.exec())