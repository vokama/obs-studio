#include "vk-message-box.hpp"

VKMessageBox::VKMessageBox(QWidget *parent, const QString& messageText)
	: VKDialog(parent)
{
	message = addText(messageText, QFont(defaultFont.family(), 13));
	message->setWordWrap(true);
	addSpace(1);
	addAcceptButton("OK");
}

void VKMessageBox::setMessage(const QString& messageText)
{
	message->setText(messageText);
}