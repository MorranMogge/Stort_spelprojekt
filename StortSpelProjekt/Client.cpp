#include "Client.h"

//returns the id of the packet
int extractPacketId(sf::Packet& packet)
{
	unsigned short packetIdentifier;

	packet >> packetIdentifier;
	//std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;
	return packetIdentifier;
}

void clientFunction(void* param)
{
	ThreadInfo* data = (ThreadInfo*)param;

	data->mp_Event.isActive = false;

	std::string temp;
	sf::Packet receivedPacket;
	while (!data->endThread)
	{
		data->socket.receive(receivedPacket); //Receives the packet
		int packetid = extractPacketId(receivedPacket);//extract the id of the packet
		
		std::string receivedString;
		unsigned short packetId;
		int playerid;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		switch (packetid)
		{
		default:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			
			data->mp_Event.isActive = true;
			data->mp_Event.pos[0] = 0.0f;
			data->mp_Event.pos[1] = 0.0f;
			data->mp_Event.pos[2] = 0.0f;
			std::cout << "TCP received data from address: " << data->socket.getRemoteAddress().toString() << std::endl;
			receivedPacket >> packetId >> playerid >> data->mp_Event.pos[0] >> data->mp_Event.pos[1] >> data->mp_Event.pos[2];
			std::cout << "data received from server: Packet id: " << std::to_string(packetId) << " player id: " << std::to_string(playerid) <<
				" x : " << std::to_string(data->mp_Event.pos[0]) << " y: " << std::to_string(data->mp_Event.pos[1]) << 
				" z: " << std::to_string(data->mp_Event.pos[2]) << std::endl;

			break;
		case 4:
			break;
		case 10://receiving id in player vector from server side

			receivedPacket >> data->playerId;
			std::cout << "player id recv: " << std::to_string(data->playerId) << std::endl;
			break;
		}
			
		
		receivedPacket.clear();
	}
}
//tempPack << packId << playerName << x << y << z;

Client::Client()
{
	this->ip = sf::IpAddress::getLocalAddress().toString();
	this->port = 2001;
	this->tmp = "empty String UwU";
	this->isConnected = false;
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);
	this->ip = ipAddress;
	this->tmp = "empty";
	this->port = 2001;
	this->isConnected = false;
}

Client::~Client()
{
	if (clientThread != nullptr)
	{
		data.endThread = true;
		clientThread->join();
		delete clientThread;
	}
}

void Client::connectToServer(std::string ipAddress, int port)
{
	if (tcpSocket.connect(ipAddress, port) != sf::Socket::Done)
	{
		std::cout << "Couldnt connect\n";
	}
	else
	{
		std::cout << "Was able to connect\n";
	}
	data.playerId = -1;
	data.endThread = false;
	this->setupThread();

}

void Client::connectToServer()
{
	if (data.socket.connect(this->ip, this->port) != sf::Socket::Done)
	{
		std::cout << "Couldnt connect\n";
	}
	else
	{
		std::cout << "Was able to connect\n";
	}

	data.playerId = 0;
	data.endThread = false;
	this->setupThread();
	this->isConnected = true;

	//this->joinThread();
	//data.socket.setBlocking(false);
}

void Client::joinThread()
{
	//data.endThread = true;
	clientThread->join();
}

void Client::setIpAndPort(std::string ipAddress, int port)
{
	this->ip = ipAddress;
	this->port = port;
}

void Client::setIdentifier(std::string id)
{

}

bool Client::setupThread()
{
	clientThread = new std::thread(clientFunction, &this->data);
	return clientThread!=nullptr;
}

std::string Client::receive()
{
	return data.receivedstring;
}

//used for sending string
void Client::sendToServerUdp(std::string stringToSend)
{
	sendPacket << stringToSend;

	
	if (sendSocket.send(sendPacket, this->ip, port) == sf::Socket::Done)
	{
		//sent
		std::cout << "sent data to server\n";
	}
	else
	{
		std::cout << "failed to send data to server\n";
	}
	sendPacket.clear();
}

//used for sending already saved MSG
void Client::sendToServerUdp()
{
	sendPacket << this->tmp;


	if (sendSocket.send(sendPacket, this->ip, port) == sf::Socket::Done)
	{
		//sent
		std::cout << "sent data to server\n";
	}
	else
	{
		std::cout << "failed to send data to server\n";
	}
	sendPacket.clear();
}

void Client::sendToServerTcp()
{
	sendPacket.clear();
	unsigned short packetIdentifier = 1;
	sendPacket << packetIdentifier << this->tmp;

	if(data.socket.send(sendPacket) != sf::Socket::Done)
	{
		//error
		std::cout << "TCP Couldnt send packet\n";
	}

	std::string s;
	unsigned short id;
	sendPacket >> id >> s;
	std::cout << "TCP sent packet: " << id << s << std::endl;
	sendPacket.clear();
}

void Client::sendToServerTcp(std::string buf)
{
	sendPacket << buf;

	if (tcpSocket.send(this->sendPacket) != sf::Socket::Done)
	{
		//error
		std::cout << "TCP Couldnt send packet\n";
	}
	else
	{
		std::string s;
		sendPacket >> s;
		std::cout << "TCP sent packet: " << s << std::endl;
	}
}

void Client::sendToServerTEMPTCP( Player*& currentPlayer)
{

	float x = currentPlayer->getPos().x;
	float y = currentPlayer->getPos().y;
	float z = currentPlayer->getPos().z;
	//int playerid = data.users[i].playerId;

	

	sendPacket.clear();
	unsigned short packetIdentifier = 3;
	
	sendPacket << packetIdentifier << data.playerId << x << y << z;

	if (data.socket.send(sendPacket) != sf::Socket::Done)
	{
		//error
		std::cout << "TCP Couldnt send packet\n";
	}
	else
	{
		float sentPos[3];

		unsigned short id;
		sendPacket >> id >> sentPos[0] >> sentPos[1] >> sentPos[2];
		std::cout << "TCP sent packet: " << id << ", " << std::to_string(sentPos[0]) << ", " << std::to_string(sentPos[1])
			<< ", " << std::to_string(sentPos[2]) << std::endl;
		sendPacket.clear();
	}
	
}

void Client::receiveFromServerTcp()
{
	receivedPacket.clear();
	if (tcpSocket.receive(receivedPacket) != sf::Socket::Done)
	{
		std::cout << "failed to receive TCP\n";
	}
	else
	{
		std::string receivedString;
		std::cout << "TCP received data from address: " << tcpSocket.getRemoteAddress().toString() << std::endl;
		receivedPacket << receivedString;
		std::cout << "data received from server: " << receivedString << std::endl;
	}
}

void Client::RECEIVEPOSITIONTEST()
{
	receivedPacket.clear();
	if (tcpSocket.receive(receivedPacket) != sf::Socket::Done)
	{
		std::cout << "failed to receive TCP\n";
	}
	else
	{
		/*std::string receivedString;
		float x = 0.0f;
		float y = 0.0f;
		std::cout << "TCP received data from address: " << tcpSocket.getRemoteAddress().toString() << std::endl;
		receivedPacket >> receivedString >> x >> y;
		std::cout << "data received from server: " << receivedString << "x : " << x << "y: " << y << std::endl;
		receivedPacket.clear();*/
	}
}

void Client::saveText(std::string text)
{
	id = text;
}

void Client::saveMsg(std::string text)
{
	this->tmp = text;
}

void Client::tempwrite()
{
	std::cout << id << std::endl;
}

movePlayerEvent Client::getMovePlayerEvent() const
{
	return data.mp_Event;
}

bool Client::getChangePlayerPos() const
{
	return data.mp_Event.isActive;
}

int Client::getport() const
{
	return this->port;
}

int Client::getPlayerId() const
{
	return this->data.playerId;
}

std::string Client::getipAdress() const
{
	return this->ip;
}

void Client::setPlayerRecv(const bool value)
{
	this->data.mp_Event.isActive = value;
}
