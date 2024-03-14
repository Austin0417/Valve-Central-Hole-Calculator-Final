/********************************************************************************
** Form generated from reading UI file 'CameraDisplayDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERADISPLAYDIALOG_H
#define UI_CAMERADISPLAYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CameraDisplayDialog
{
public:
    QPushButton *capture_img_btn;
    QLabel *live_video_display_label;

    void setupUi(QDialog *CameraDisplayDialog)
    {
        if (CameraDisplayDialog->objectName().isEmpty())
            CameraDisplayDialog->setObjectName("CameraDisplayDialog");
        CameraDisplayDialog->resize(1200, 650);
        capture_img_btn = new QPushButton(CameraDisplayDialog);
        capture_img_btn->setObjectName("capture_img_btn");
        capture_img_btn->setGeometry(QRect(540, 610, 111, 31));
        live_video_display_label = new QLabel(CameraDisplayDialog);
        live_video_display_label->setObjectName("live_video_display_label");
        live_video_display_label->setGeometry(QRect(80, 10, 1050, 600));
        live_video_display_label->setStyleSheet(QString::fromUtf8("background: black;"));

        retranslateUi(CameraDisplayDialog);

        QMetaObject::connectSlotsByName(CameraDisplayDialog);
    } // setupUi

    void retranslateUi(QDialog *CameraDisplayDialog)
    {
        CameraDisplayDialog->setWindowTitle(QCoreApplication::translate("CameraDisplayDialog", "Dialog", nullptr));
        capture_img_btn->setText(QCoreApplication::translate("CameraDisplayDialog", "Capture Image", nullptr));
        live_video_display_label->setText(QCoreApplication::translate("CameraDisplayDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraDisplayDialog: public Ui_CameraDisplayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERADISPLAYDIALOG_H
