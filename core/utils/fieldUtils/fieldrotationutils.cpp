#include "fieldrotationutils.h"

int FieldRotationUtils::getRotation(const int &fieldId)
{
	if(fieldId % 10 == 0)
	{
		// it's a corner field

		if(fieldId / 10 < 2)
		{
			// it's a bottom corner field -> no rotation
			return 0;
		}
		else
		{
			// it's a top corner field -> upside down
			return 180;
		}
	}

	// every other field -> calculate rotation
	return (fieldId / 10) * 90;
}
