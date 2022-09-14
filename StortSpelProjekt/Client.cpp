#include "Client.h"

void clientFunction(void* param)
{
	ThreadInfo* data = (ThreadInfo*)param;
	std::string temp;
	sf::Packet receivedPacket;
	while (!data->endThread)
	{
		data->socket.receive(receivedPacket); //Receives the packet
		if (receivedPacket >> temp)	//Checks whether or not the packet contains data
		{
			data->receivedstring = temp;
		}
		receivedPacket.clear();
	}
}

Client::Client()
{
	this->ip = sf::IpAddress::getLocalAddress().toString();
	this->port = 2001;
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);
	this->ip = ipAddress;
	this->tmp = "empty";
	this->port = 2001;
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
	data.socket.connect(ipAddress, port);
}

void Client::connectToServer()
{
	tcpSocket.connect(this->ip, this->port);
	//data.socket.setBlocking(false);
}

void Client::joinThread()
{
	data.endThread = true;
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
	sendPacket << this->tmp;

	if(tcpSocket.send(sendPacket) != sf::Socket::Done)
	{
		//error
		std::cout << "TCP Couldnt send packet\n";
	}

	std::string s;
	sendPacket >> s;
	std::cout << "TCP sent packet: " << s << std::endl;
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

int Client::getport() const
{
	return this->port;
}

std::string Client::getipAdress() const
{
	return this->ip;
}