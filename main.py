import sys

from PySide6.QtCore import QSettings
from PySide6.QtWidgets import QApplication

# Main entry point of the application.
if __name__ == "__main__":
    app = QApplication(sys.argv)

    app.setOrganizationName("Datrix")
    app.setOrganizationDomain("www.datrixgroup.com")
    app.setApplicationName("SpectroscoPy")
    QSettings.setDefaultFormat(QSettings.Format.IniFormat)

    from app.MainWindow import MainWindow

    main_window = MainWindow()

    main_window.show()

    # Start the application event loop.
    sys.exit(app.exec())