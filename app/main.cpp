#include <QCoreApplication>
#include <core/baselogic.h>
//#include <QLoggingCategory>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//QLoggingCategory::setFilterRules("*.debug=true");

	BaseLogic start(&a);
	start.logic();

	a.exit();
	return 0;
}
