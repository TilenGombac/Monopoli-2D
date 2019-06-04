#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QString>

class ErrorHandler
{
public:
	static void message(const QString &title, const QString &text);
	static void log(const QString &title, const QString &text = "");

};

#endif // ERRORHANDLER_H
