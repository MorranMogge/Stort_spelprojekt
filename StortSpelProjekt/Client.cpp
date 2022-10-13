#include "Client.h"

//returns the id of the packet
int extractPacketId(sf::Packet& packet)
{
	unsigned short packetIdentifier;

	packet >> packetIdentifier;
	//std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;
	return packetIdentifier;
}

int extractPacketId(const char data[])
{
	int idPacket;
	memcpy(&idPacket, data, sizeof(int));

	return idPacket;
}

//extracts the data thats been received
void* extractData(const char data[], const std::size_t& recvSize)
{
	void* d = malloc(recvSize);
	memcpy(d, data, recvSize);
	return d;
}

void clientFunction(void* param)
{
	ThreadInfo* data = (ThreadInfo*)param;

	while (!data->endThread)
	{
		char receivedData[256];
		std::size_t recvSize;
		data->socket.receive(receivedData, 256, recvSize); //Receives the packet
		//int packetid = extractPacketId(receivedData);//extract the id of the packet
		//
		//void* dataStruct = extractData(receivedData, recvSize);
		//testPosition *tst = nullptr;
		//idProtocol* protocol = nullptr;

		
		data->circularBuffer->addData(receivedData, recvSize);
	}
}

Client::Client()
{
	this->ip = sf::IpAddress::getLocalAddress().toString();
	this->port = 2001;
	this->tmp = "empty String UwU";
	this->isConnected = false;
	this->data.circularBuffer = new CircularBuffer();
	this->data.playerId = 0;
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);
	this->ip = ipAddress;
	this->tmp = "empty";
	this->port = 2001;
	this->isConnected = false;
	this->data.circularBuffer = new CircularBuffer();
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
	data.endThread = false;
	this->setupThread();
	this->isConnected = true;

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

	testPosition testStruct;

	testStruct.packetId = 10;
	testStruct.playerId = this->getPlayerId();
	testStruct.x = currentPlayer->getPos().x;
	testStruct.y = currentPlayer->getPos().y;
	testStruct.z = currentPlayer->getPos().z;
	//int playerid = data.users[i].playerId;

	

	sendPacket.clear();
	//unsigned short packetIdentifier = 3;
	
	//returns the nr of bytes sent
	std::size_t sentSize;

	if (data.socket.send(&testStruct,sizeof(testStruct), sentSize) != sf::Socket::Done)
	{
		//error
		std::cout << "TCP Couldnt send packet\n";
	}
	else
	{

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

bool Client::getIfConnected()
{
	return this->isConnected;
}

int Client::getport() const
{
	return this->port;
}

int Client::getPlayerId() const
{
	return this->data.playerId;
}

int Client::initTEMPPLAYERS()
{
	int temp = -1;

	char receivedData[256];
	std::size_t recvSize;
	data.socket.receive(receivedData, 256, recvSize);

	char* charData[sizeof(idProtocol)];
	memcpy(charData, receivedData, sizeof(idProtocol));
	idProtocol* idProto = (idProtocol*)charData;
	temp = idProto->assignedPlayerId;

	std::cout << "Client.cpp, ASSIGNED ID BY SERVER: " << std::to_string(idProto->assignedPlayerId) << std::endl;
	
	return temp;
}

CircularBuffer*& Client::getCircularBuffer()
{
	return data.circularBuffer;
}

std::string Client::getipAdress() const
{
	return this->ip;
}
