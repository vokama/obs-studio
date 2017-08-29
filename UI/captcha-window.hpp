#pragma once

#include "vk-dialog.hpp"

#define BUFFER_SIZE (256 * 1024)  /* 256 KB */

struct ResponseData {
	uchar data[BUFFER_SIZE];
	size_t size;
};

class CaptchaWindow : public VKDialog
{
	Q_OBJECT

public:
	explicit CaptchaWindow(QWidget *parent = NULL, const char *url = "");
	QString getUserInput();
	void reloadCaptcha(const char *url);
private:
	void downloadImgData(ResponseData *imgData, const char *url);
private:
	const QLineEdit *inputField;
	QLabel *captchaLabel;
};