#include "baselogic.h"
#include "listcomparison.h"

BaseLogic::BaseLogic(QObject *parent, QCoreApplication* app)
	: QObject{parent}, appPtr{app}
{
	appName = "ListComparison";
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
	// First option
	QCommandLineOption one(QStringList() << "o" << "one", "Show one");
	// Second option
	QCommandLineOption two(QStringList() << "t" << "two", "Show two");

	parser.addOption(one);
	parser.addOption(two);

	return 0;
}

int BaseLogic::checkingTheCommandAndRunningIt(QCommandLineParser& parser)
{
	ListComparison libObject;

	if (parser.isSet("one")){
		QTextStream stream(stdout); // Для разнообразия, вместо qInfo.
		QStringList result = libObject.getBranchList();
		stream << "Branch: " << result.size() << " [" << result.join(",") << "]" << Qt::endl;
	} else if (parser.isSet("two")){
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
