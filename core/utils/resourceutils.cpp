#include "resourceutils.h"

#include <QFile>

QString ResourceUtils::getStyleSheet(QString path)
{
	QFile file(path);
	file.open(QFile::ReadOnly);

	QString style;

	if(file.isOpen())
	{
		style = file.readAll();

		file.close();
	}

	return style;
}
