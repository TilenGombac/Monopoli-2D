#include "packet.h"

#include "core/utils/errorUtils/errorhandler.h"

Packet::Packet(QByteArray data)
{
	setData(data);
}

Packet::Packet(const PacketType &type)
{
	setType(type);
}

void Packet::setType(const PacketType &type)
{
	this->type = type;

	if(type != PacketType::INVALID)
	{
		QString packetNumber;

		if((int)this->type < 10)
		{
			packetNumber += "0";
		}

		packetNumber += QString::number((int)this->type);

		data.insert(0, packetNumber);
	}
}

void Packet::setData(QByteArray data)
{
	this->data.clear();

	setType(determineType(data));



	this->data.append(data.mid(2));
}

PacketType Packet::getType()
{
	return type;
}

QByteArray Packet::getData(bool includeHeader)
{
	if(includeHeader)
	{
		// Packet size at the beginning
		int size = this->data.size();

		QByteArray tmp = data;

		tmp.prepend((char*)&size, sizeof(int));

		return tmp;
	}

	return data.mid(2);
}

PacketType Packet::determineType(QByteArray data)
{
	QString packetType = data.mid(0, 2);
	bool ok;
	int number = packetType.toInt(&ok);

	if(ok)
		return (PacketType)number;

	return PacketType::INVALID;
}
