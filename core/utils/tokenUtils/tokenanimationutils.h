#ifndef TOKENANIMATIONUTILS_H
#define TOKENANIMATIONUTILS_H

#include <QVariantAnimation>

class TokenAnimationUtils
{
public:
	static int getDuration(const int &fieldId, const int &targetFieldId,
							const int &timePerField);

	static int getFieldDelta(const int &fieldId, const int &targetFieldId);

	static QVariantAnimation::KeyValues
	generateKeyValues(const int &targetId, int fieldId,
						const QPixmap &pixmap);

};

#endif // TOKENANIMATIONUTILS_H
