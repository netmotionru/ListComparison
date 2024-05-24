#ifndef REST_H
#define REST_H

#include <QObject>
#include <HEADERS_QT.h>

class Rest : public QObject
{
	Q_OBJECT

	QNetworkAccessManager manager;
	QEventLoop            loop;

public:
	explicit Rest(QObject *parent = nullptr);
	QNetworkReply* requestGetWait(const QNetworkRequest& inputRequest);
	QNetworkReply* requestPostWait(const QNetworkRequest& inputRequest);
	QNetworkReply* requestPutWait(const QNetworkRequest& inputRequest);
	QNetworkReply* requestDeleteWait(const QNetworkRequest& inputRequest);

};

#endif // REST_H
