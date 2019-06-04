#include "updateboardpacket.h"

#include "core/game/board/board.h"
#include "core/game/entity/dice.h"
#include "core/game/entity/card.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][1 - PlayerTurn][1 - speedingString][1 - First][1 - Second]
 *		[2*cardsSize - cardIds]
 */

UpdateBoardPacket::UpdateBoardPacket(Board *board)
	: Packet(PacketType::UPDATE_BOARD)
{
	// The fields get updated by the PlayerActionPacket
	// Player turn changing handled by the SetTurnPacket

	int playerTurn = board->getPlayerTurn();

	int speedingCounter = board->getSpeedingCounter();

	int firstValue  = board->getDice()->getFirst();
	int secondValue = board->getDice()->getSecond();

	QString playerTurnString = QString::number(playerTurn);

	QString speedingString = QString::number(speedingCounter);

	QString firstString  = QString::number(firstValue);
	QString secondString = QString::number(secondValue);

	data.append(playerTurnString);
	data.append(speedingString);
	data.append(firstString);
	data.append(secondString);

	QString cardId;

	for(Card *card : board->getCards())
	{
		cardId = "";

		if(card->getId() < 10)
			cardId += "0";

		cardId += QString::number(card->getId());

		data.append(cardId);
	}

	// Players are handled by PlayerActionPackets, MovePackets
	//    and RequestPaymentPackets
}

UpdateBoardPacket::UpdateBoardPacket(QByteArray data)
	: Packet(data)
{

}

bool UpdateBoardPacket::getUpdatedBoard(Board *current)
{
	if(current == nullptr)
		current = new Board();

	QByteArray boardData = getData();

	bool ok;

	QString playerTurnString = boardData.mid(0, 1);

	int playerTurn = playerTurnString.toInt(&ok);

	if(!ok)
		return false;

	QString speedingString = boardData.mid(1, 1);

	int speedingCounter = speedingString.toInt(&ok);

	if(!ok)
		return false;

	QString firstString  = boardData.mid(2, 1);
	QString secondString = boardData.mid(3, 1);

	int firstValue = firstString.toInt(&ok);

	if(!ok)
		return false;

	int secondValue = secondString.toInt(&ok);

	if(!ok)
		return false;

	QString cardIdString;

	int cardId;

	QVector<Card*> cards = current->getCards();

	for(int i = 4;  i < boardData.size(); i += 2)
	{
		cardIdString = boardData.mid(i, 2);

		cardId = cardIdString.toInt(&ok);

		if(!ok)
			return false;

		int index = (i - 4) / 2;

		for(int j = 0; j < cards.size(); j++)
		{
			if(cards[j]->getId() == cardId)
			{
				Card *tmp = cards[j];
				cards[j] = cards[index];
				cards[index] = tmp;

				break;
			}
		}
	}

	current->setCards(cards);
	current->getDice()->setValue(firstValue, secondValue);
	current->setSpeeding(speedingCounter);
	current->setPlayerTurn(playerTurn);

	return true;
}
