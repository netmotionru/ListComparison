#include "listcomparison.h"

ListComparison::ListComparison()
{
}

QStringList ListComparison::getBranchList()
{
	QStringList branchList;
	QNetworkRequest request;
	request.setUrl(QUrl(QString("%1%2").arg(URL_API, URL_BRANCH_TREE)));

	qInfo() << "Requesting branches, it takes about 10 seconds.";
	QNetworkReply* reply = restObj.requestGetWait(request);

	// Parse
	if (reply->error()){
		qDebug() << "ListComparison::getBranchList Error:" << reply->errorString() << reply->readAll();
	}
	QByteArray message = reply->readAll();
	QVariantList branchesVarinatList = QJsonDocument::fromJson(message).object().value("branches").toArray().toVariantList();
	for (auto brancheVariant : branchesVarinatList){
		branchList.append(brancheVariant.toString());
	}

	reply->deleteLater();
	return branchList;
}
