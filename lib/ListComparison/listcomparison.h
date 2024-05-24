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

public:
	ListComparison();
	QStringList getBranchList();
};

#endif // LISTCOMPARISON_H
