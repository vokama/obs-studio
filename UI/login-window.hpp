#pragma once

#include <jansson.h>

#include "vk-dialog.hpp"

enum LoginError {
	Ok,
	NotValid,
	NeedCaptcha,
	NeedCode,
	WrongCode,
	UnknownError
};

class LoginWindow : public VKDialog
{
	Q_OBJECT

public:
	explicit LoginWindow();
	std::string getToken();
public Q_SLOTS:
	virtual void accept();
private:
	/*  methods to login  */
	bool login(const QString& username, const QString& password);
	LoginError getLoginStatus(json_t *reply);
private:
	const QLineEdit *usernameField;
	const QLineEdit *passwordField;
	std::string access_token;
};