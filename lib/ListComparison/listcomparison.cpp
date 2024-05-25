#include "listcomparison.h"
#include <iostream>

ListComparison::ListComparison()
{
}

QByteArray ListComparison::getBranchAndCheck(QString branch)
{

	QNetworkRequest request;
	request.setUrl(QUrl(QString("%1%2/%3").arg(URL_API, URL_BRANCH_BINARY_PACKAGES, branch)));

	QNetworkReply* reply = restObj.requestGetWait(request);

	QByteArray responce;
	if (reply->error()){
		responce = reply->readAll();
		qDebug() << "ListComparison::getDifferenceBranch Error:" << reply->errorString() << responce;
		if (responce.contains("unknown package set name")){
			QJsonArray responceArray = QJsonDocument::fromJson(responce).object().value("validation_message").toArray();
			qInfo().noquote() << responceArray.first().toString() << "," << responceArray.last().toString();
		}
		return QByteArray();
	}
	responce = reply->readAll();
	if (!responce.contains("packages")){
		qInfo() << "Packet retrieval error, branch name may be incorrect" << responce;
		return QByteArray();
	}

	reply->deleteLater();
	return responce;
}

QHash<QString, QStringList> ListComparison::creatingHashArch(QByteArray& responce, QHash<QString, QStringList>& hashArch)
{
	QJsonArray packagesArray = QJsonDocument::fromJson(responce).object().value("packages").toArray();
	for (auto package : packagesArray){
		QJsonObject packageObj = package.toObject();
		QString archName = packageObj.value("arch").toString();
		QString packageName = packageObj.value("name").toString();
		if (hashArch[archName].contains(packageName)) continue;
		hashArch[archName].append(packageName);
	}

	return hashArch;
}

QStringList ListComparison::getBranchList()
{
	/*
	 * Creates a binary list of all available branches for packages.altlinux.org.
	 * This list does not apply to the "/export/branch_binary_packages" API.
	 */

	QStringList branchList;
	QNetworkRequest request;
	request.setUrl(QUrl(QString("%1%2").arg(URL_API, URL_BRANCH_TREE)));

	qInfo() << "Requesting branches, it takes about 10 seconds.";
	QNetworkReply* reply = restObj.requestGetWait(request);

	// Parse
	if (reply->error()){
		qDebug() << "ListComparison::getBranchList Error:" << reply->errorString() << reply->readAll();
	}
	QByteArray responce = reply->readAll();
	QVariantList branchesVarinatList = QJsonDocument::fromJson(responce).object().value("branches").toArray().toVariantList();
	for (auto brancheVariant : branchesVarinatList){
		branchList.append(brancheVariant.toString());
	}

	reply->deleteLater();
	return branchList;
}

QHash<QString, QStringList> ListComparison::getBranchInfo(QString branch)
{
	/*
	 * Shows a list of architectures and the number of packages on the selected branch.
	 */

	qInfo().noquote() << QString("Getting the list of binary packages of the [%1] branch.").arg(branch);
	QByteArray responceBranch = getBranchAndCheck(branch);
	if (responceBranch.isEmpty()) return QHash<QString, QStringList>();

	QHash<QString, QStringList> hashArch;
	creatingHashArch(responceBranch, hashArch);

	return hashArch;
}

QHash<QString, QStringList> ListComparison::getDifferenceBranch(QStringList branchList)
{
	/*
	 * Creates a list of binary packages that are in the first branch but not in the second branch
	 */

	qInfo().noquote() << QString("Getting the list of binary packages of the first [%1] branch.").arg(branchList.first());
	QByteArray responceFirstBranch = getBranchAndCheck(branchList.first());
	if (responceFirstBranch.isEmpty()) return QHash<QString, QStringList>();

	qInfo().noquote() << QString("Getting the list of binary packages of the second [%1] branch.").arg(branchList.last());
	QByteArray responceSecondBranch = getBranchAndCheck(branchList.last());
	if (responceSecondBranch.isEmpty()) return QHash<QString, QStringList>();

	qInfo() << "Process the received lists of binary packets";
	QHash<QString, QStringList> firstHashArch;
	creatingHashArch(responceFirstBranch, firstHashArch);
	QHash<QString, QStringList> secondHashArch;
	creatingHashArch(responceSecondBranch, secondHashArch);

	for (auto i = secondHashArch.begin(), end = secondHashArch.end(); i != end; ++i){
		if (!firstHashArch.contains(i.key())) continue;

		// Sorted lists are required for std::set_difference
		firstHashArch[i.key()].sort();
		secondHashArch[i.key()].sort();

		QStringList tempResult;
		std::set_difference(i.value().cbegin(), i.value().cend(),
							firstHashArch[i.key()].cbegin(), firstHashArch[i.key()].cend(),
							std::inserter(tempResult, tempResult.end()));
		firstHashArch[i.key()].clear();
		firstHashArch[i.key()] = tempResult;
	}

	return firstHashArch;
}
