#include <vk-api/methods.h>
#include <QRegExpValidator>

#include "code2fa-window.hpp"
#include "obs-app.hpp"

Code2faWindow::Code2faWindow(QWidget *parent)
	: VKDialog(parent)
{
	QValidator *inputValidator;

	addText(QTStr("2fa.NeedCode"), QFont(defaultFont.family(), 13));

	addSpace(1);

	inputField = addInputField(QTStr("2fa.EnterCode"));
	inputValidator = new QRegExpValidator(QRegExp("\\d{6,8}"), this);
	inputField->setValidator(inputValidator);
	
	acceptButton = addAcceptButton(QTStr("OK"));
	acceptButton->setEnabled(false);

	QObject::connect(inputField, SIGNAL(textEdited(QString)),
			this, SLOT(validateInput(QString)));
}

void Code2faWindow::validateInput(QString text)
{
	if (inputField->hasAcceptableInput())
		acceptButton->setEnabled(true);
	else
		acceptButton->setEnabled(false);
}

QString Code2faWindow::getCode()
{
	return inputField->text();
}