# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'ImagesWidget.ui'
##
## Created by: Qt User Interface Compiler version 6.9.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QGraphicsView, QHBoxLayout, QSizePolicy,
    QWidget)

class Ui_ImagesWidget(object):
    def setupUi(self, ImagesWidget):
        if not ImagesWidget.objectName():
            ImagesWidget.setObjectName(u"ImagesWidget")
        ImagesWidget.resize(400, 300)
        self.MainLayout = QHBoxLayout(ImagesWidget)
        self.MainLayout.setObjectName(u"MainLayout")
        self.OriginalImg = QGraphicsView(ImagesWidget)
        self.OriginalImg.setObjectName(u"OriginalImg")

        self.MainLayout.addWidget(self.OriginalImg)

        self.ProcessedImg = QGraphicsView(ImagesWidget)
        self.ProcessedImg.setObjectName(u"ProcessedImg")

        self.MainLayout.addWidget(self.ProcessedImg)


        self.retranslateUi(ImagesWidget)

        QMetaObject.connectSlotsByName(ImagesWidget)
    # setupUi

    def retranslateUi(self, ImagesWidget):
        ImagesWidget.setWindowTitle(QCoreApplication.translate("ImagesWidget", u"Form", None))
    # retranslateUi

