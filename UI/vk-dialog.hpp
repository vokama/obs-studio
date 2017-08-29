#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/*  An abstract class for creating stylish dialogues  */
class VKDialog : public QDialog
{
	Q_OBJECT

protected:
	explicit VKDialog(QWidget *parent = NULL);
	virtual ~VKDialog() = 0;
protected:
	/*  methods to build interface  */
	QLabel *addText(const QString &, const QFont &);
	QLabel *addImage(const QPixmap &);
	QLineEdit *addInputField(const QString &,
			QLineEdit::EchoMode = QLineEdit::Normal);
	QPushButton *addAcceptButton(const QString &);
	void addSpace(int linesCount);
protected:
	QPalette defaultPal;
	QFont defaultFont;
private:
	QVBoxLayout mainLayout;
	int spacing;
};