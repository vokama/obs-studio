#pragma once

#include "auth-oauth.hpp"

class VkOAuthLogin : public OAuthLogin {
	Q_OBJECT

public:
	VkOAuthLogin(QWidget *parent, const std::string &url, bool token);

public slots:
	virtual void urlChanged(const QString &url) override;
};

class VkAuth : public OAuthStreamKey {
	Q_OBJECT

	virtual bool RetryLogin() override;

public:
	VkAuth(const Def &d);

	static std::shared_ptr<Auth> Login(QWidget *parent);
};