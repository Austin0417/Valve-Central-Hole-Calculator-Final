/********************************************************************************
** Form generated from reading UI file 'CameraConfirmation.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERACONFIRMATION_H
#define UI_CAMERACONFIRMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_CameraConfirmationDialog
{
public:
    QPushButton *camera_confirm_btn;
    QPushButton *camera_retake_btn;
    QToolButton *camera_crop_btn;
    QPushButton *clear_crop_lines_btn;
    QLineEdit *captured_image_name_line_edit;
    QLabel *label;
    QRadioButton *jpg_btn_;
    QRadioButton *png_btn_;

    void setupUi(QDialog *CameraConfirmationDialog)
    {
        if (CameraConfirmationDialog->objectName().isEmpty())
            CameraConfirmationDialog->setObjectName("CameraConfirmationDialog");
        CameraConfirmationDialog->resize(1229, 650);
        camera_confirm_btn = new QPushButton(CameraConfirmationDialog);
        camera_confirm_btn->setObjectName("camera_confirm_btn");
        camera_confirm_btn->setGeometry(QRect(1140, 60, 81, 24));
        camera_retake_btn = new QPushButton(CameraConfirmationDialog);
        camera_retake_btn->setObjectName("camera_retake_btn");
        camera_retake_btn->setGeometry(QRect(1140, 40, 81, 24));
        camera_crop_btn = new QToolButton(CameraConfirmationDialog);
        camera_crop_btn->setObjectName("camera_crop_btn");
        camera_crop_btn->setGeometry(QRect(10, 20, 51, 41));
        clear_crop_lines_btn = new QPushButton(CameraConfirmationDialog);
        clear_crop_lines_btn->setObjectName("clear_crop_lines_btn");
        clear_crop_lines_btn->setGeometry(QRect(1140, 20, 81, 24));
        captured_image_name_line_edit = new QLineEdit(CameraConfirmationDialog);
        captured_image_name_line_edit->setObjectName("captured_image_name_line_edit");
        captured_image_name_line_edit->setGeometry(QRect(540, 620, 113, 22));
        label = new QLabel(CameraConfirmationDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(420, 620, 111, 16));
        jpg_btn_ = new QRadioButton(CameraConfirmationDialog);
        jpg_btn_->setObjectName("jpg_btn_");
        jpg_btn_->setGeometry(QRect(670, 620, 89, 20));
        png_btn_ = new QRadioButton(CameraConfirmationDialog);
        png_btn_->setObjectName("png_btn_");
        png_btn_->setGeometry(QRect(730, 620, 89, 20));

        retranslateUi(CameraConfirmationDialog);

        QMetaObject::connectSlotsByName(CameraConfirmationDialog);
    } // setupUi

    void retranslateUi(QDialog *CameraConfirmationDialog)
    {
        CameraConfirmationDialog->setWindowTitle(QCoreApplication::translate("CameraConfirmationDialog", "Dialog", nullptr));
        camera_confirm_btn->setText(QCoreApplication::translate("CameraConfirmationDialog", "Confirm", nullptr));
        camera_retake_btn->setText(QCoreApplication::translate("CameraConfirmationDialog", "Retake Image", nullptr));
        camera_crop_btn->setText(QCoreApplication::translate("CameraConfirmationDialog", "Crop", nullptr));
        clear_crop_lines_btn->setText(QCoreApplication::translate("CameraConfirmationDialog", "Clear Crop Lines", nullptr));
        label->setText(QCoreApplication::translate("CameraConfirmationDialog", "Saved Image Name", nullptr));
        jpg_btn_->setText(QCoreApplication::translate("CameraConfirmationDialog", ".jpg", nullptr));
        png_btn_->setText(QCoreApplication::translate("CameraConfirmationDialog", ".png", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraConfirmationDialog: public Ui_CameraConfirmationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERACONFIRMATION_H
