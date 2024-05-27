#ifndef BASELOGIC_H
#define BASELOGIC_H

#include <QObject>
#include <HEADERS_QT.h>

class BaseLogic : public QObject
{
	Q_OBJECT

	QCoreApplication* appPtr       = nullptr;
	QString           appName      = "ListComparison";
	QString           appVersion   = VERSION;
	QString           jsonFileName = "outputJson.txt"; // For option "json"

public:
	explicit BaseLogic(QObject *parent = nullptr, QCoreApplication* app = nullptr);
	int settingsCoreApplication();
	int settingsCommandLineOption(QCommandLineParser& parser);
	int addingCommandLineOption(QCommandLineParser& parser);
	int checkingTheCommandAndRunningIt(QCommandLineParser& parser);
	int logic();

};

#endif // BASELOGIC_H
