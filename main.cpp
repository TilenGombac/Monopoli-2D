#include <QApplication>

#include "core/applicationloader.h"
#include "core/ui/mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QApplication::setWindowIcon(QIcon(":/img/img/icon.png"));

	MainWindow mainWindow;

	ApplicationLoader::load(&mainWindow);

	int rc = a.exec();

	return rc;
}
