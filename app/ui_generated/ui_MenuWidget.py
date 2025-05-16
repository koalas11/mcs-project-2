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
        self.ImageFileLabel = QLabel(MenuWidget)
        self.ImageFileLabel.setObjectName(u"ImageFileLabel")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ImageFileLabel.sizePolicy().hasHeightForWidth())
        self.ImageFileLabel.setSizePolicy(sizePolicy)

        self.verticalLayout.addWidget(self.ImageFileLabel)

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

        self.GridLayout = QHBoxLayout()
        self.GridLayout.setObjectName(u"GridLayout")
        self.GridLayout.setContentsMargins(-1, 0, -1, -1)
        self.GridSizeSpinBox = QSpinBox(MenuWidget)
        self.GridSizeSpinBox.setObjectName(u"GridSizeSpinBox")
        self.GridSizeSpinBox.setMinimum(1)
        self.GridSizeSpinBox.setMaximum(100)
        self.GridSizeSpinBox.setValue(2)

        self.GridLayout.addWidget(self.GridSizeSpinBox)

        self.ToggleGridButton = QPushButton(MenuWidget)
        self.ToggleGridButton.setObjectName(u"ToggleGridButton")

        self.GridLayout.addWidget(self.ToggleGridButton)


        self.verticalLayout.addLayout(self.GridLayout)

        self.GridColorLayout = QHBoxLayout()
        self.GridColorLayout.setObjectName(u"GridColorLayout")
        self.GridColorLayout.setContentsMargins(-1, 0, -1, -1)
        self.GridColorLabel = QLabel(MenuWidget)
        self.GridColorLabel.setObjectName(u"GridColorLabel")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.GridColorLabel.sizePolicy().hasHeightForWidth())
        self.GridColorLabel.setSizePolicy(sizePolicy1)

        self.GridColorLayout.addWidget(self.GridColorLabel)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Minimum)

        self.GridColorLayout.addItem(self.horizontalSpacer)

        self.GridColorValue = QPushButton(MenuWidget)
        self.GridColorValue.setObjectName(u"GridColorValue")
        self.GridColorValue.setText(u"")

        self.GridColorLayout.addWidget(self.GridColorValue)


        self.verticalLayout.addLayout(self.GridColorLayout)

        self.line_3 = QFrame(MenuWidget)
        self.line_3.setObjectName(u"line_3")
        self.line_3.setFrameShape(QFrame.Shape.HLine)
        self.line_3.setFrameShadow(QFrame.Shadow.Sunken)

        self.verticalLayout.addWidget(self.line_3)

        self.CutOffThresholdLayout = QHBoxLayout()
        self.CutOffThresholdLayout.setObjectName(u"CutOffThresholdLayout")
        self.CutOffThresholdLayout.setContentsMargins(-1, 0, -1, -1)
        self.CutOffThresholdLabel = QLabel(MenuWidget)
        self.CutOffThresholdLabel.setObjectName(u"CutOffThresholdLabel")

        self.CutOffThresholdLayout.addWidget(self.CutOffThresholdLabel)

        self.CutOffThresholdSpinBox = QSpinBox(MenuWidget)
        self.CutOffThresholdSpinBox.setObjectName(u"CutOffThresholdSpinBox")
        self.CutOffThresholdSpinBox.setMaximum(100)

        self.CutOffThresholdLayout.addWidget(self.CutOffThresholdSpinBox)


        self.verticalLayout.addLayout(self.CutOffThresholdLayout)

        self.line_2 = QFrame(MenuWidget)
        self.line_2.setObjectName(u"line_2")
        self.line_2.setFrameShape(QFrame.Shape.HLine)
        self.line_2.setFrameShadow(QFrame.Shadow.Sunken)

        self.verticalLayout.addWidget(self.line_2)

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
        self.ImageFileLabel.setText(QCoreApplication.translate("MenuWidget", u"Image File:", None))
        self.ImageBrowseButton.setText(QCoreApplication.translate("MenuWidget", u"Browse", None))
        self.ToggleGridButton.setText(QCoreApplication.translate("MenuWidget", u"Disable Grid", None))
        self.GridColorLabel.setText(QCoreApplication.translate("MenuWidget", u"Current Grid Color:", None))
        self.CutOffThresholdLabel.setText(QCoreApplication.translate("MenuWidget", u"Cut Off Threshold", None))
        self.ApplyButton.setText(QCoreApplication.translate("MenuWidget", u"Apply", None))
    # retranslateUi

