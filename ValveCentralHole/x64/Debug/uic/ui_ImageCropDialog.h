/********************************************************************************
** Form generated from reading UI file 'ImageCropDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGECROPDIALOG_H
#define UI_IMAGECROPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ImageCropDialog
{
public:
    QPushButton *image_crop_ok;

    void setupUi(QDialog *ImageCropDialog)
    {
        if (ImageCropDialog->objectName().isEmpty())
            ImageCropDialog->setObjectName("ImageCropDialog");
        ImageCropDialog->resize(1229, 650);
        ImageCropDialog->setStyleSheet(QString::fromUtf8("background: white;"));
        image_crop_ok = new QPushButton(ImageCropDialog);
        image_crop_ok->setObjectName("image_crop_ok");
        image_crop_ok->setGeometry(QRect(580, 610, 101, 31));
        image_crop_ok->setStyleSheet(QString::fromUtf8("background: white;"));

        retranslateUi(ImageCropDialog);

        QMetaObject::connectSlotsByName(ImageCropDialog);
    } // setupUi

    void retranslateUi(QDialog *ImageCropDialog)
    {
        ImageCropDialog->setWindowTitle(QCoreApplication::translate("ImageCropDialog", "Dialog", nullptr));
        image_crop_ok->setText(QCoreApplication::translate("ImageCropDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageCropDialog: public Ui_ImageCropDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGECROPDIALOG_H
