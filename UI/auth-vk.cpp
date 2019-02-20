#include "auth-vk.hpp"

#include <iostream>
#include <fstream>

#include <qt-wrappers.hpp>

#include "window-basic-main.hpp"
#include "remote-text.hpp"

#include <json11.hpp>

#include "obf.h"
#include "ui-config.h"

using namespace json11;

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

bool VkAuth::GetChannelInfo()
try {
	std::string client_id = TWITCH_CLIENTID;
	deobfuscate_str(&client_id[0], TWITCH_HASH);

	if (!GetToken(VK_TOKEN_URL, client_id, VK_SCOPE_VERSION))
		return false;
	if (token.empty())
		return false;
	if (!key_.empty())
		return true;

	std::string output;
	std::string error;

	bool success = false;

	auto func = [this, &success, &output, &error]() {
		success = GetRemoteFile(
			(std::string("https://api.vk.com/method/video.startStreaming?access_token=") + token + "&v=5.92").c_str(),
			output,
			error,
			nullptr,
			nullptr,
			nullptr,
			std::vector<std::string>(),
			nullptr,
			5);
	};

	ExecuteFuncSafeBlockMsgBox(
		func,
		QTStr("Auth.LoadingChannel.Title"),
		QTStr("Auth.LoadingChannel.Text").arg(service()));
	if (!success || output.empty())
		throw ErrorInfo("Failed to get text from remote", error);

	Json json = Json::parse(output, error);
	if (!error.empty())
		throw ErrorInfo("Failed to parse json", error);

	error = json["error"].string_value();
	if (!error.empty())
		throw ErrorInfo(error, json["error_description"].string_value());

	url_ = json["response"]["stream"]["url"].string_value();
	key_ = json["response"]["stream"]["key"].string_value();

	return true;
} catch (ErrorInfo info) {
	QString title = QTStr("Auth.ChannelFailure.Title");
	QString text = QTStr("Auth.ChannelFailure.Text")
		.arg(service(), info.message.c_str(), info.error.c_str());

	QMessageBox::warning(OBSBasic::Get(), title, text);

	blog(LOG_WARNING, "%s: %s: %s",
		__FUNCTION__,
		info.message.c_str(),
		info.error.c_str());
	return false;
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

	if (auth->GetChannelInfo()) {
		return auth;
	}

	return nullptr;
}

bool VkAuth::RetryLogin()
{
	VkOAuthLogin login(OBSBasic::Get(), VK_AUTH_URL, false);
	if (login.exec() == QDialog::Rejected) {
		return false;
	}

	std::shared_ptr<VkAuth> auth = std::make_shared<VkAuth>(vkDef);
	std::string client_id = VK_CLIENTID;
	deobfuscate_str(&client_id[0], VK_HASH);

	return GetToken(VK_TOKEN_URL, std::string(VK_CLIENTID) + "&client_secret=pPdfQ5fn33P0wUX5KKPa&redirect_uri=" VK_REDIRECT_URI, VK_SCOPE_VERSION,
			QT_TO_UTF8(login.GetCode()), true);
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
	blog(LOG_INFO, "VK Auth urlChanged: %s", QT_TO_UTF8(url));
	std::string uri = "code=";
	int code_idx = url.indexOf(uri.c_str());
	if (code_idx == -1)
		return;

	if (url.left(31) != "https://oauth.vk.com/blank.html")
		return;

	code_idx += (int)uri.size();

	int next_idx = url.indexOf("&", code_idx);
	if (next_idx != -1)
		code = url.mid(code_idx, next_idx - code_idx);
	else
		code = url.right(url.size() - code_idx);

	accept();
}

void VkAuth::OnStreamConfig()
{
	OBSBasic *main = OBSBasic::Get();
	obs_service_t *service = main->GetService();

	obs_data_t *settings = obs_service_get_settings(service);

	obs_data_set_string(settings, "key", key_.c_str());
	obs_data_set_string(settings, "server", url_.c_str());
	obs_service_update(service, settings);

	obs_data_release(settings);
}