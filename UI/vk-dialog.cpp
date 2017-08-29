#include "vk-dialog.hpp"

QLabel *VKDialog::addText(const QString &text, const QFont &font)
{
	QLabel *label = new QLabel(text);
	label->setFont(font);
	label->setAlignment(Qt::AlignHCenter);
	mainLayout.addWidget(label);
	return label;
}

QLabel *VKDialog::addImage(const QPixmap& image)
{
	QLabel *label = new QLabel();
	label->setPixmap(image);
	label->setAlignment(Qt::AlignHCenter);
	mainLayout.addWidget(label);
	return label;
}

QLineEdit *VKDialog::addInputField(const QString &placeholderText,
		QLineEdit::EchoMode mode)
{
	QLineEdit *field = new QLineEdit;
	field->setFixedSize(270, 30);
	field->setPlaceholderText(placeholderText);
	field->setFont(QFont("Segoe UI", 10));
	field->setStyleSheet("padding-left: 1ex;");
	field->setEchoMode(mode);
	mainLayout.addWidget(field, 0, Qt::AlignHCenter);
	return field;
}

QPushButton *VKDialog::addAcceptButton(const QString &text)
{
	QPushButton *button = new QPushButton(text);
	button->setFixedSize(270, 30);
	button->setStyleSheet(
		"QPushButton { \
			background-color: #6888ad;\
			color: white;\
			border: none;\
		}\
		QPushButton:pressed { \
			background-color:#507299;\
		}\
		QPushButton:disabled { \
			background-color:#b3c3d5;\
		}");
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(accept()));
	mainLayout.addWidget(button, 0, Qt::AlignHCenter);
	return button;
}

void VKDialog::addSpace(int linesCount)
{
	mainLayout.addSpacing(linesCount * spacing);
}

VKDialog::VKDialog(QWidget *parent)
	: QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	spacing = 13;

	defaultPal.setColor(QPalette::Window, "#FFFFFF");
	setPalette(defaultPal);
	
	defaultFont.setFamily("Segoe UI Light");
	defaultFont.setPointSize(10);
	setFont(defaultFont);

	mainLayout.setAlignment(Qt::AlignVCenter);
	setLayout(&mainLayout);
}

VKDialog::~VKDialog() {};