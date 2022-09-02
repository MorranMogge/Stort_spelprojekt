#include <iostream>

#include <SFML/Network.hpp>
#include <chrono>

int main()
{
	std::string s = "empty";
	// Group the variables to send into a packet
	sf::Packet packet;
	sf::Packet receivedPacket;
	float frequency = 30.f;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	std::cout << ip.toString() << "\n";
	sf::TcpSocket socket;
	std::string connectionType, mode;

	char buffer[2000];
	size_t recieved;
	std::string text = "Connect to: ";

	std::string receivedstring;

	std::cout << "Starting server!\n";

	sf::TcpListener listener;
	listener.listen(2001);
	std::cout << "listening\n";

	listener.accept(socket);
	std::cout << "Accepted client write a message: ";

	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();

	int counter = 0;
	while (true)
	{
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > 1.0f / frequency)
		{
			std::getline(std::cin, s);
			//s = std::to_string(counter++);
			packet << s;

			socket.send(packet);
			packet.clear();
			start = std::chrono::system_clock::now();
		}
		
	}
	/*if (connectionType == "s")
	{
		sf::TcpListener listener;
		listener.listen(2001);
		listener.accept(socket);
		while (true)
		{
			std::getline(std::cin, s);
			packet << s;

			socket.send(packet);

			socket.receive(receivedPacket);
			if (receivedPacket >> receivedstring)
			{
				// Data extracted successfully...
				std::cout << receivedstring << std::endl;
				receivedstring.clear();
				packet.clear();
				receivedPacket.clear();
			}
		}
	}
	else if (connectionType == "c")
	{
		socket.setBlocking(false);
		socket.connect(ip, 2001);

		while (true)
		{
			std::getline(std::cin, s);
			packet << s;

			socket.send(packet);
			socket.receive(receivedPacket);
			if (receivedPacket >> receivedstring)
			{
				// Data extracted successfully...
				std::cout << receivedstring << std::endl;
				receivedstring.clear();
				packet.clear();
				receivedPacket.clear();
			}
		}
	}*/


    getchar();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
