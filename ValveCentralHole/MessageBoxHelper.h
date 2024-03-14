#pragma once
#include <QMessageBox>



class MessageBoxHelper
{
public:
	MessageBoxHelper();
	static int ShowErrorDialog(const QString& error_msg);
};

