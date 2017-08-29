#pragma once

#include "vk-dialog.hpp"

class Code2faWindow : public VKDialog
{
	Q_OBJECT

public:
	explicit Code2faWindow(QWidget *parent = NULL);
	QString getCode();

public Q_SLOTS:
	void validateInput(QString);

private:
	QLineEdit *inputField;
	QPushButton *acceptButton;
};