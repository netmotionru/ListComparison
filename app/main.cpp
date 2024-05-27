#include <QCoreApplication>
#include <core/baselogic.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	BaseLogic start(&a);
	start.logic();

	a.exit();
	return 0;
}
