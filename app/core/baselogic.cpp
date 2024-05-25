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

	// Add-on option for the difference option. Forms output as JSON.
	// Example: >./app -j -d <FIRST_BRANCH> -d <SECOND_BRANCH>
	QCommandLineOption showDifferencePackagesJson(QStringList() << "j" << "json",
								  "Add-on option for the difference option. Forms output as JSON.\n"
								  "Example: -j -d <FIRST_BRANCH> -d <SECOND_BRANCH>");

	// Additional <BRANCH> argument for output in the help information
	parser.addPositionalArgument("<BRANCH>", "Branch name");

	parser.addOption(showAllBranch);
	parser.addOption(getBranchAndShow);
	parser.addOption(showDifferencePackages);
	parser.addOption(showDifferencePackagesJson);

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
			stream << "The number of branches is incorrectly, there should be one." << Qt::endl;
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
			stream << "The number of branches is incorrect, there should be two." << Qt::endl;
			parser.showHelp(1);
		} else {
			QHash<QString, QStringList> result = libObject.getDifferenceBranch(branchList);
			if (result.isEmpty()) parser.showHelp(1);

			// Если выбрана дополнительная опция --json
			if (parser.isSet("json")){
				QJsonObject architecturesObj;
				QFile fileObj(jsonFileName);

				if (!fileObj.open(QFile::WriteOnly | QFile::Truncate)){
					stream << "Error creating or opening " << jsonFileName << " file." << Qt::endl;
				}

				QTextStream streamToFile(&fileObj);

				for (auto i = result.cbegin(), end = result.cend(); i != end; ++i){
					QJsonArray packagesArray;
					for (auto package : i.value()){
						packagesArray.append(package);
					}
					architecturesObj[i.key()] = packagesArray;
				}
				QJsonDocument architecturesDoc(architecturesObj);
				QByteArray outputJsonByteData = architecturesDoc.toJson(QJsonDocument::Compact);
				streamToFile << outputJsonByteData;
				stream << outputJsonByteData << Qt::endl;
				stream << "The result is saved to the " << jsonFileName << " file" << Qt::endl;

			}else{
				for (auto i = result.cbegin(), end = result.cend(); i != end; ++i){
					stream << "Arch: [" << i.key() << "] packages: " << i.value().size() << Qt::endl;
				}
			}
		}

	} else if (parser.isSet("json")){
		stream << "This option must be used in conjunction with the difference option." << Qt::endl;
		parser.showHelp(1);

	} else {
		// Additional processing is probably never called, since QCommandLineParser has an error handler.
		stream << "There is no logic for the specified parameter." << Qt::endl;
		parser.showHelp(1);
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
