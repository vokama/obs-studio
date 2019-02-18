#include "auth-vk.hpp"

#include <iostream>
#include <fstream>

#include <qt-wrappers.hpp>

#include "obf.h"
#include "ui-config.h"

#define VK_REDIRECT_URI "https://oauth.vk.com/blank.html"
#define VK_AUTH_URL \
	"https://oauth.vk.com/authorize?" \
			"client_id=5554299" \
			"&redirect_uri=" VK_REDIRECT_URI \
			"&display=popup" \
			"&response_type=code" \
			"&v=5.92"
#define VK_TOKEN_URL \
	"https://oauth.vk.com/access_token"

#define VK_SCOPE_VERSION (16 + 8192 + 262144)

static Auth::Def vkDef = {
	"VK",
	Auth::Type::OAuth_StreamKey
};

VkAuth::VkAuth(const Def &d)
	: OAuthStreamKey(d)
{

}

std::shared_ptr<Auth> VkAuth::Login(QWidget *parent)
{
	VkOAuthLogin login(parent, VK_AUTH_URL, false);
	if (login.exec() == QDialog::Rejected) {
		return nullptr;
	}

	std::shared_ptr<VkAuth> auth = std::make_shared<VkAuth>(vkDef);

	std::string client_id = VK_CLIENTID;
	deobfuscate_str(&client_id[0], VK_HASH);

	if (!auth->GetToken(VK_TOKEN_URL, std::string(VK_CLIENTID) + "&client_secret=pPdfQ5fn33P0wUX5KKPa&redirect_uri=" VK_REDIRECT_URI, VK_SCOPE_VERSION,
				QT_TO_UTF8(login.GetCode())))
	{
		return nullptr;
	}

	blog(LOG_INFO, "VK auth token: %s", auth->token.c_str());

	return auth;
}

bool VkAuth::RetryLogin()
{
	return true;
}

static std::shared_ptr<Auth> CreateVkAuth()
{
	return std::make_shared<VkAuth>(vkDef);
}

static void DeleteCookies()
{

}

void RegisterVkAuth()
{
	OAuth::RegisterOAuth(vkDef, CreateVkAuth, VkAuth::Login, DeleteCookies);
}

VkOAuthLogin::VkOAuthLogin(QWidget *parent, const std::string &url, bool token)
	: OAuthLogin(parent, url, token)
{}

void VkOAuthLogin::urlChanged(const QString &url)
{
	std::string uri = "code=";
	int code_idx = url.indexOf(uri.c_str());
	if (code_idx == -1)
		return;

	if (url.left(32) != "https://oauth.vk.com/blank.html#")
		return;

	code_idx += (int)uri.size();

	code = url.right(url.size() - code_idx);

	accept();
}