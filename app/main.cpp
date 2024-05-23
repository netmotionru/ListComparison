#include <QCoreApplication>
#include <core/baselogic.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	BaseLogic start;
	start.logic();

	return a.exec();
}
