#include "stdafx.h"
#include "Client.h"

//returns the id of the packet
int extractPacketId(sf::Packet& packet)
{
	unsigned short packetIdentifier;

	packet >> packetIdentifier;
	//std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;
	return packetIdentifier;
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
		
		data->circularBuffer->addData(receivedData, recvSize);
	}
}

Client::Client()
{
	this->ip = sf::IpAddress::getLocalAddress().toString();
	this->port = 2001;
	this->tmp = "empty String UwU";
	this->isConnected = false;
	this->data.circularBuffer = new CircularBufferClient();
	this->data.playerId = 0;
	
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);
	this->ip = ipAddress;
	this->tmp = "empty";
	this->port = 2001;
	this->isConnected = false;
	this->data.circularBuffer = new CircularBufferClient();
	
}

Client::~Client()
{
	if (clientThread != nullptr)
	{
		data.endThread = true;
		clientThread->join();
		delete clientThread;
	}
	delete this->data.circularBuffer;
}

void Client::setClientId(int nr)
{
	this->data.playerId = nr;
}

bool Client::connectToServer(std::string ipAddress, int port)
{
	std::cout << "Test\n";
	std::cout << "CB: " << sizeof(data.circularBuffer) << "\nUdpSocket: " << sizeof(sf::UdpSocket) << "\nTcpSocket: " << sizeof(sf::TcpSocket)
		<< "\nSocket selector: " << sizeof(sf::SocketSelector) << "\nPacket: " << sizeof(sf::Packet) << "\nThread: " << sizeof(std::thread) << "\n";
	bool connected = false;
	this->ip = ipAddress;
	this->port = port;
	std::cout << "Tries to connect to server ip adress: " << ipAddress << std::endl;
	if (tcpSocket.connect(ipAddress, port) != sf::Socket::Done)
	{
		std::cout << "Couldnt connect\n";
	}
	else
	{
		connected = true;
		std::cout << "Was able to connect\n";
	}

	//if it doesnt connect returns false
	if (!connected) return connected;
	
	this->ip = ipAddress;
	this->port = port;
	data.endThread = false;
	this->setupThread();
	this->isConnected = true;

	//returns true if connected
	return connected;
}

bool Client::connectToServer()
{
	std::cout << "Test\n";
	std::cout << "CB: " << sizeof(data.circularBuffer) << "\nUdpSocket: " << sizeof(sf::UdpSocket) << "\nTcpSocket: " << sizeof(sf::TcpSocket)
		<< "\nSocket selector: " << sizeof(sf::SocketSelector) << "\nPacket: " << sizeof(sf::Packet) << "\nThread: " << sizeof(std::thread) << "\n";
	bool connected = false;
	std::cout << "Tries to connect to server ip adress: " << this->ip << std::endl;

	if (data.socket.connect(this->ip, this->port) != sf::Socket::Done)
	{
		std::cout << "Couldnt connect\n";
	}
	else
	{
		connected = true;
		std::cout << "Was able to connect\n";
	}
	//if it doesnt connect returns false
	if (!connected) return connected;

	data.endThread = false;
	this->setupThread();
	this->isConnected = true;

	//returns true if connected
	return connected;
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

CircularBufferClient*& Client::getCircularBuffer()
{
	return data.circularBuffer;
}

std::string Client::getipAdress() const
{
	return this->ip;
}
