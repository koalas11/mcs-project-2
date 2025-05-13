# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'MenuWidget.ui'
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
from PySide6.QtWidgets import (QApplication, QFrame, QHBoxLayout, QLabel,
    QLineEdit, QPushButton, QSizePolicy, QSpacerItem,
    QSpinBox, QVBoxLayout, QWidget)

class Ui_MenuWidget(object):
    def setupUi(self, MenuWidget):
        if not MenuWidget.objectName():
            MenuWidget.setObjectName(u"MenuWidget")
        MenuWidget.resize(400, 300)
        self.verticalLayout = QVBoxLayout(MenuWidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.ImageLabel = QLabel(MenuWidget)
        self.ImageLabel.setObjectName(u"ImageLabel")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ImageLabel.sizePolicy().hasHeightForWidth())
        self.ImageLabel.setSizePolicy(sizePolicy)

        self.verticalLayout.addWidget(self.ImageLabel)

        self.ImageFileLayout = QHBoxLayout()
        self.ImageFileLayout.setObjectName(u"ImageFileLayout")
        self.ImagePathLineEdit = QLineEdit(MenuWidget)
        self.ImagePathLineEdit.setObjectName(u"ImagePathLineEdit")

        self.ImageFileLayout.addWidget(self.ImagePathLineEdit)

        self.ImageBrowseButton = QPushButton(MenuWidget)
        self.ImageBrowseButton.setObjectName(u"ImageBrowseButton")

        self.ImageFileLayout.addWidget(self.ImageBrowseButton)


        self.verticalLayout.addLayout(self.ImageFileLayout)

        self.line = QFrame(MenuWidget)
        self.line.setObjectName(u"line")
        self.line.setFrameShape(QFrame.Shape.HLine)
        self.line.setFrameShadow(QFrame.Shadow.Sunken)

        self.verticalLayout.addWidget(self.line)

        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(-1, 0, -1, -1)
        self.GridSizeSpinBox = QSpinBox(MenuWidget)
        self.GridSizeSpinBox.setObjectName(u"GridSizeSpinBox")
        self.GridSizeSpinBox.setMinimum(2)
        self.GridSizeSpinBox.setMaximum(100)
        self.GridSizeSpinBox.setValue(5)

        self.horizontalLayout_2.addWidget(self.GridSizeSpinBox)

        self.ToggleGridButton = QPushButton(MenuWidget)
        self.ToggleGridButton.setObjectName(u"ToggleGridButton")

        self.horizontalLayout_2.addWidget(self.ToggleGridButton)


        self.verticalLayout.addLayout(self.horizontalLayout_2)

        self.GridColorLabel = QLabel(MenuWidget)
        self.GridColorLabel.setObjectName(u"GridColorLabel")

        self.verticalLayout.addWidget(self.GridColorLabel)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(-1, 0, -1, -1)
        self.GridColorValueLabel = QLabel(MenuWidget)
        self.GridColorValueLabel.setObjectName(u"GridColorValueLabel")
        self.GridColorValueLabel.setText(u"")

        self.horizontalLayout.addWidget(self.GridColorValueLabel)

        self.GridColorBrowseButton = QPushButton(MenuWidget)
        self.GridColorBrowseButton.setObjectName(u"GridColorBrowseButton")

        self.horizontalLayout.addWidget(self.GridColorBrowseButton)


        self.verticalLayout.addLayout(self.horizontalLayout)

        self.ApplyButton = QPushButton(MenuWidget)
        self.ApplyButton.setObjectName(u"ApplyButton")

        self.verticalLayout.addWidget(self.ApplyButton)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)


        self.retranslateUi(MenuWidget)

        QMetaObject.connectSlotsByName(MenuWidget)
    # setupUi

    def retranslateUi(self, MenuWidget):
        MenuWidget.setWindowTitle(QCoreApplication.translate("MenuWidget", u"Form", None))
        self.ImageLabel.setText(QCoreApplication.translate("MenuWidget", u"File: ", None))
        self.ImageBrowseButton.setText(QCoreApplication.translate("MenuWidget", u"PushButton", None))
        self.ToggleGridButton.setText(QCoreApplication.translate("MenuWidget", u"PushButton", None))
        self.GridColorLabel.setText(QCoreApplication.translate("MenuWidget", u"Current Grid Color:", None))
        self.GridColorBrowseButton.setText(QCoreApplication.translate("MenuWidget", u"PushButton", None))
        self.ApplyButton.setText(QCoreApplication.translate("MenuWidget", u"Apply", None))
    # retranslateUi

