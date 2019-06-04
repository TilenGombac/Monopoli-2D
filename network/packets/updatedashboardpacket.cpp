#include "updatedashboardpacket.h"

/*
 * Packet composition:
 * [Bytes - Part]
 *
 * [6 - Header][X - Actions]
 */


UpdateDashboardPacket::UpdateDashboardPacket(QVector<PlayerAction> actions)
	: Packet(PacketType::UPDATE_DASHBOARD)
{
	int actionId;
	QString actionIdString;

	for(PlayerAction action : actions)
	{
		actionId = (int)action;

		actionIdString = QString::number(actionId);

		this->data.append(actionIdString);
	}
}

UpdateDashboardPacket::UpdateDashboardPacket(QByteArray data)
	: Packet(data)
{

}

QVector<PlayerAction> UpdateDashboardPacket::getActions()
{
	QVector<PlayerAction> actions;

	QByteArray actionsData = this->getData();

	bool ok;

	int actionId;

	PlayerAction action;

	for(int i = 0; i < actionsData.size(); i++)
	{
		actionId = actionsData.mid(i, 1).toInt(&ok);

		if(ok)
		{
			action = (PlayerAction)actionId;

			actions.append(action);
		}
	}

	return actions;
}
