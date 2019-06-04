#ifndef TEXTLINE_H
#define TEXTLINE_H

#include <QTextEdit>
#include <QKeyEvent>

class TextLine : public QTextEdit
{
	Q_OBJECT

	int length;

public:
	TextLine(QWidget *parent = 0);
	void setLengthLimit(const int &length);

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
	void checkLimit();

signals:
	void enterPressed();

};

#endif // TEXTLINE_H
