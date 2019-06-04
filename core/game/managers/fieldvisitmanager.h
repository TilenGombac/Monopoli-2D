#ifndef FIELDVISITMANAGER_H
#define FIELDVISITMANAGER_H

#include <QObject>

class Field;
class Player;
class BoardManager;

class FieldVisitManager : public QObject
{
	Q_OBJECT

	BoardManager *boardManager;

public:
	FieldVisitManager(BoardManager *setBoardManager);

public slots:
	void handleVisit();

};

#endif // FIELDVISITMANAGER_H
