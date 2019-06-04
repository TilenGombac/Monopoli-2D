#include "tokenanimationutils.h"

#include "core/utils/tokenUtils/tokengeometryutils.h"

int TokenAnimationUtils::getDuration(const int &fieldId,
											  const int &targetFieldId,
												const int &timePerField)
{
	int fieldDelta = getFieldDelta(fieldId, targetFieldId);

	return fieldDelta * timePerField;
}

int TokenAnimationUtils::getFieldDelta(const int &fieldId,
												const int &targetFieldId)
{
	// Normal scenario
	if(targetFieldId > fieldId)
	{
		return (targetFieldId - fieldId);
	}

	// Going all the way around
	if(targetFieldId == fieldId)
	{
		return 40;
	}

	// The token passes through start

	int delta = 0;

	int i = fieldId;

	while(i != targetFieldId)
	{
		delta++;

		i++;

		// 39 is the max field id, should change to 0 when it reaches 40
		i = i % 40;
	}

	return delta;
}

QVariantAnimation::KeyValues
TokenAnimationUtils::generateKeyValues(const int &targetId, int fieldId,
										 const QPixmap &pixmap)
{
	QVariantAnimation::KeyValues keyValues;
	QVariantAnimation::KeyValue keyValue;

	QRect rect;

	int delta = TokenAnimationUtils::getFieldDelta(fieldId, targetId);

	for(int i = 0; i <= delta; i++)
	{
		// Only need to set keyframes on the starting field,
		//	corner fields and target field
		if((i == 0) || (i == delta) || (fieldId % 10 == 0))
		{
			qreal step = i / qreal(delta);

			rect = TokenGeometryUtils::getRect(fieldId, pixmap);

			keyValue = QPair<qreal, QVariant> (step, rect);

			keyValues.append(keyValue);
		}

		fieldId++;

		// Max fieldId value is 39
		fieldId = fieldId % 40;
	}

	return keyValues;
}
