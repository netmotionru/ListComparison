#include "rest.h"

Rest::Rest(QObject *parent)
	: QObject{parent}
{
	connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
}

QNetworkReply* Rest::requestGetWait(const QNetworkRequest& inputRequest)
{
	qDebug() << "Rest::requestGetWait" << inputRequest.url().toString();

	QNetworkReply* reply = manager.get(inputRequest);
	if (loop.exec()) qCritical() << "Rest::requestGetWait - Error, loop.exec()";
	return reply;
}

QNetworkReply* Rest::requestPostWait(const QNetworkRequest& inputRequest)
{
	qDebug() << "Rest::requestPostWait" << inputRequest.url().toString();

	QNetworkReply* reply = manager.post(inputRequest, QByteArray());
	if (loop.exec()) qCritical() << "Rest::requestPostWait - Error, loop.exec()";
	return reply;
}

QNetworkReply* Rest::requestPutWait(const QNetworkRequest& inputRequest)
{
	qDebug() << "Rest::requestPutWait" << inputRequest.url().toString();

	QNetworkReply* reply = manager.put(inputRequest, QByteArray());
	if (loop.exec()) qCritical() << "Rest::requestPutWait - Error, loop.exec()";
	return reply;
}

QNetworkReply* Rest::requestDeleteWait(const QNetworkRequest& inputRequest)
{
	qDebug() << "Rest::requestDeleteWait" << inputRequest.url().toString();

	QNetworkReply* reply = manager.deleteResource(inputRequest);
	if (loop.exec()) qCritical() << "Rest::requestDeleteWait - Error, loop.exec()";
	return reply;
}
