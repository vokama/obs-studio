#pragma once

#include "vk-dialog.hpp"

class VKMessageBox : public VKDialog
{
	Q_OBJECT

public:
	explicit VKMessageBox(QWidget *parent = NULL,
			const QString& messageText = "");

	void setMessage(const QString &);
private:
	QLabel *message;
};