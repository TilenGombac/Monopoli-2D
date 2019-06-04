#include "textline.h"

TextLine::TextLine(QWidget *parent)
	: QTextEdit(parent)
{
	length = -1;
}

void TextLine::setLengthLimit(const int &length)
{
	this->length = length;

	connect(this, SIGNAL(textChanged()), this, SLOT(checkLimit()));
}

void TextLine::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Return)
	{
		emit enterPressed();
	}
	else
	{
		QTextEdit::keyPressEvent(event);
	}
}

void TextLine::checkLimit()
{
	if(length != -1)
	{
		if(this->toPlainText().length() > length)
		{
			QString tmp = this->toPlainText();

			tmp.chop(1);

			this->setText(tmp);

			QTextCursor cursor = this->textCursor();
			cursor.movePosition(QTextCursor::End);

			this->setTextCursor(cursor);
		}
	}
}
