#include "baselogic.h"
#include "listcomparison.h"

BaseLogic::BaseLogic(QObject *parent, QCoreApplication* app)
	: QObject{parent}, appPtr{app}
{

}

int BaseLogic::settingsCoreApplication()
{
	QCoreApplication::setApplicationName(appName);
	QCoreApplication::setApplicationVersion(appVersion);

	return 0;
}

int BaseLogic::settingsCommandLineOption(QCommandLineParser& parser)
{
	parser.setApplicationDescription(QString("%1 helper").arg(appName));
	parser.addHelpOption();
	parser.addVersionOption();

	return 0;
}

int BaseLogic::addingCommandLineOption(QCommandLineParser& parser)
{
	// Show all available branches.
	// example: >./app -s
	QCommandLineOption showAllBranch(QStringList() << "s" << "show",
								  "Show all available branches");

	// Gets and shows the list of binary packages of the selected branch. Only one branch can be specified.
	// Example: >./app -g <BRANCH>
	QCommandLineOption getBranchAndShow(QStringList() << "g" << "get",
								 "Get the list of binary packages of the selected branch",
								 "BRANCH");

	// Gets the binary packages of two branches and shows a list of packages that are in the first branch but not in the second branch.
	// If inversion is necessary, just change the names of the branches. It is necessary to specify the names of the two branches.
	// Example: >./app -d <FIRST_BRANCH> -d <SECOND_BRANCH>
	QCommandLineOption showDifferencePackages(QStringList() << "d" << "difference",
								  "Gets and compares binary packages of two branches and outputs a list of packages that are in the first branch but not in the second branch.\n"
								  "Example: -d <FIRST_BRANCH> -d <SECOND_BRANCH>",
								  "BRANCH");

	// Additional <BRANCH> argument for output in the help information
	parser.addPositionalArgument("<BRANCH>", "Branch name");

	parser.addOption(showAllBranch);
	parser.addOption(getBranchAndShow);
	parser.addOption(showDifferencePackages);

	return 0;
}

int BaseLogic::checkingTheCommandAndRunningIt(QCommandLineParser& parser)
{
	ListComparison libObject; // Shared dynamic library
	QTextStream stream(stdout);

	// Processing of the "show" option
	if (parser.isSet("show")){
		QStringList result = libObject.getBranchList();
		stream << "Branch: " << result.size() << " [" << result.join(",") << "]" << Qt::endl;

	// Processing of the "get" option
	} else if (parser.isSet("get")){
		QStringList branchList = parser.values("get");
		if (branchList.size() != 1){
			qInfo() << "The number of branches is incorrectly, there should be one.";
			parser.showHelp(1);
		} else {
			QHash<QString, QStringList> hashArch = libObject.getBranchInfo(branchList.first());
			if (hashArch.isEmpty()) parser.showHelp(1);

			stream << "|- BRANCH: " << branchList.first() << Qt::endl;
			for (auto i = hashArch.cbegin(), end = hashArch.cend(); i != end; ++i){
				stream << "|-- Arch: [" << i.key() << "] packages: " << i.value().size() << Qt::endl;
			}
		}

	// Processing of the "difference" option
	} else if (parser.isSet("difference")){
		QStringList branchList = parser.values("difference");
		if (branchList.size() != 2){
			qInfo() << "The number of branches is incorrect, there should be two.";
			parser.showHelp(1);
		} else {
			QHash<QString, QStringList> result = libObject.getDifferenceBranch(branchList);
			if (result.isEmpty()) parser.showHelp(1);

			for (auto i = result.cbegin(), end = result.cend(); i != end; ++i){
				stream << "Arch: [" << i.key() << "] packages: " << i.value().size() << Qt::endl;
			}
		}

	} else {
		// Additional processing is probably never called, since QCommandLineParser has an error handler.
		qDebug() << "There is no logic for the specified parameter";
		return 1;
	}

	return 0;
}

int BaseLogic::logic()
{
	// Base settings
	settingsCoreApplication();

	// CommandLine logic
	QCommandLineParser parser;
	settingsCommandLineOption(parser);
	addingCommandLineOption(parser);
	parser.process(*appPtr);
	checkingTheCommandAndRunningIt(parser);

	return 0;
}
