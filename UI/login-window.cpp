#include <vk-api/methods.h>

#include "qt-wrappers.hpp"
#include "login-window.hpp"
#include "obs-app.hpp"
#include "vk-message-box.hpp"
#include "captcha-window.hpp"
#include "code2fa-window.hpp"

LoginError LoginWindow::getLoginStatus(json_t *reply)
{
	const char *error = JSON_GET_STRING(reply, "error");
	const char *errorDescription =
			JSON_GET_STRING(reply, "error_description");

	if (!error)
		return Ok;
	if (!strcmp(error, "invalid_client"))
		return NotValid;
	if (!strcmp(error, "need_captcha"))
		return NeedCaptcha;
	if (!strcmp(error, "need_validation"))
		return NeedCode;
	if (!strcmp(errorDescription, "wrong code"))
		return WrongCode;

	return UnknownError;
}

bool LoginWindow::login(const QString& username, const QString& password)
{
	json_t *reply = NULL;
	QString captchaKey;
	QString code;
	const char *captchaUrl;
	const char *captchaSid;

	VKMessageBox errMsgBox(this);
	CaptchaWindow captchaWin(this);
	Code2faWindow codeWin(this);

	reply = login_simple(QT_TO_UTF8(username), QT_TO_UTF8(password));
	if (!reply) {
		errMsgBox.setMessage(QTStr("Login.Error.Network"));
		errMsgBox.exec();
		return false;
	}

readReply:
	switch (getLoginStatus(reply)) {
	case Ok:
		access_token = JSON_GET_STRING(reply, "access_token");
		json_decref(reply);
		return true;
	case NotValid:
		errMsgBox.setMessage(QTStr("Login.Error.NotValid"));
		errMsgBox.exec();
		json_decref(reply);
		return false;
	case NeedCaptcha:
		captchaSid = JSON_GET_STRING(reply, "captcha_sid");
		captchaUrl = JSON_GET_STRING(reply, "captcha_img");

		captchaWin.reloadCaptcha(captchaUrl);

		if (!captchaWin.exec())
			return false;

		captchaKey = captchaWin.getUserInput();

		json_decref(reply);
		reply = login_captcha(
				QT_TO_UTF8(username), QT_TO_UTF8(password),
				captchaSid, QT_TO_UTF8(captchaKey));
		goto readReply;
	case WrongCode:
	case NeedCode:
		if (!codeWin.exec())
			return false;

		code = codeWin.getCode();

		json_decref(reply);
		reply = login_2fa(QT_TO_UTF8(username), QT_TO_UTF8(password),
				QT_TO_UTF8(code));
		goto readReply;
	case UnknownError:
	default:
		errMsgBox.setMessage(QTStr("Login.Error.Unknown"));
		errMsgBox.exec();
		json_decref(reply);
		return false;
	}
}

std::string LoginWindow::getToken()
{
	return access_token;
}

void LoginWindow::accept()
{
	QString username = usernameField->text();
	QString password = passwordField->text();

	if (login(username, password))
		QDialog::accept();
}

LoginWindow::LoginWindow()
{
	resize(701, 414);

	addText(QTStr("Login.Subtitle"), QFont(defaultFont.family(), 18));
	addSpace(2);
	addText(QTStr("Login.Login"), QFont(defaultFont.family(), 13));
	addSpace(1);
	usernameField = addInputField(QTStr("Login.Username"));
	passwordField = addInputField(QTStr("Login.Password"),
			QLineEdit::Password);
	addAcceptButton(QTStr("Login.Enter"));
}