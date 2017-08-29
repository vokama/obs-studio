#include <vk-api/methods.h>
#include <curl/curl.h>

#include "captcha-window.hpp"
#include "obs-app.hpp"

size_t httpWrite(void *input, size_t memb_size, size_t nmemb,
		ResponseData *target)
{
	size_t inputsize = memb_size * nmemb;

	if (target->size + inputsize >= BUFFER_SIZE)
		return 0;

	memcpy(target->data + target->size, input, inputsize);
	target->size += inputsize;

	return inputsize;
}

CaptchaWindow::CaptchaWindow(QWidget *parent, const char *url)
	: VKDialog(parent)
{
	QPixmap img;
	ResponseData imgData = {};

	addText(QTStr("Captcha.NeedCaptcha"), QFont(defaultFont.family(), 13));

	downloadImgData(&imgData, url);
	img.loadFromData(imgData.data, imgData.size);
	captchaLabel = addImage(img);

	addSpace(1);

	inputField = addInputField(QTStr("Captcha.EnterCaptcha"));

	addAcceptButton(QTStr("OK"));
}

void CaptchaWindow::downloadImgData(ResponseData *imgData, const char *url)
{
	CURL *curl = NULL;
	CURLcode code;
	struct curl_tlssessioninfo *tlsinfo;
	long response_code;
	char errorstr[CURL_ERROR_SIZE];

	curl = curl_easy_init();
	if (!curl)
		goto error;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpWrite);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)imgData);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorstr);

#if LIBCURL_VERSION_NUM >= 0x072400
	curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPN, 0);
#endif

	/* Certificate revocation checking produces constant connection errors
	   for some users */
	curl_easy_getinfo(curl, CURLINFO_TLS_SSL_PTR, &tlsinfo);
	if (tlsinfo->backend == CURLSSLBACKEND_SCHANNEL)
		curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS,
					CURLSSLOPT_NO_REVOKE);

	code = curl_easy_perform(curl);
	if (code != CURLE_OK) {
		blog(LOG_WARNING, "Captcha unavailable, CURL error %d (%s)",
								code, errorstr);
		goto error;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	if (response_code >= 400) {
		blog(LOG_WARNING, "Captcha unavailable, HTTP/%ld",
							response_code);
		goto error;
	}
error:
	curl_easy_cleanup(curl);
}

void CaptchaWindow::reloadCaptcha(const char *url)
{
	QPixmap img;
	ResponseData imgData = {};

	downloadImgData(&imgData, url);
	img.loadFromData(imgData.data, imgData.size);
	captchaLabel->setPixmap(img);
}

QString CaptchaWindow::getUserInput()
{
	return inputField->text();
}