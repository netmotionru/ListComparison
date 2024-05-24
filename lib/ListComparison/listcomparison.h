#ifndef LISTCOMPARISON_H
#define LISTCOMPARISON_H

#include "ListComparison_global.h"
#include "network/rest.h"

class LISTCOMPARISON_EXPORT ListComparison
{
	Rest restObj;

	const QString URL_API                    = "https://rdb.altlinux.org/api";
	const QString URL_BRANCH_TREE            = "/export/branch_tree";
	const QString URL_BRANCH_BINARY_PACKAGES = "/export/branch_binary_packages";

protected:
	QByteArray                  getBranchAndCheck(QString branch);
	QHash<QString, QStringList> creatingHashArch(QByteArray& responce, QHash<QString, QStringList>& hashArch);

public:
	ListComparison();
	QStringList                 getBranchList();
	QHash<QString, QStringList> getBranchInfo(QString branch);
	QHash<QString, QStringList> getDifferenceBranch(QStringList branchList);
};

#endif // LISTCOMPARISON_H
