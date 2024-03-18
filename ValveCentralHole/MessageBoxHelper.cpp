#include "MessageBoxHelper.h"

MessageBoxHelper::MessageBoxHelper() {}


int MessageBoxHelper::ShowErrorDialog(const QString& error_msg) {
	QMessageBox* messageBox = new QMessageBox(nullptr);
	messageBox->setIcon(QMessageBox::Critical);
	messageBox->setText(error_msg);
	QPushButton* ok_btn = messageBox->addButton("OK", QMessageBox::AcceptRole);

	return messageBox->exec();
}


int MessageBoxHelper::ShowOkDialog(const QString& ok_msg)
{
	QMessageBox* messageBox = new QMessageBox(nullptr);
	messageBox->setIcon(QMessageBox::Information);
	messageBox->setText(ok_msg);
	QPushButton* ok_btn = messageBox->addButton("OK", QMessageBox::AcceptRole);

	return messageBox->exec();
}