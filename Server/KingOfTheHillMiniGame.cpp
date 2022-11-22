#include "KingOfTheHillMiniGame.h"

KingOfTheHillMiniGame::KingOfTheHillMiniGame(serverData& data, const short &nrOfPlayers)
	:kingOfTheHillOrigo(-50,0,0), radius(30), nrOfPlayers(nrOfPlayers), team1Score(0), team2Score(0), pointsToAdd(10), time(5), goalScore(100)
{
	sendKingOfTheHillZone(data);
	this->timer = std::chrono::system_clock::now();
	this->timerToSend = std::chrono::system_clock::now();
	timerSend =  30.0f;
}

void KingOfTheHillMiniGame::sendKingOfTheHillZone(serverData& data)
{
	CreateZone zone;
	zone.xPos = kingOfTheHillOrigo.x;
	zone.yPos = kingOfTheHillOrigo.y;
	zone.zPos = kingOfTheHillOrigo.z;
	zone.scale = radius;
	zone.packetId = PacketType::CREATEZONE;
	sendBinaryDataAllPlayers<CreateZone>(zone, data);
}

void KingOfTheHillMiniGame::update(serverData& data)
{
	static float xPos;
	static float yPos;
	static float zPos;
	static DirectX::XMFLOAT3 playerPos;
	for(int i = 0; i < this->nrOfPlayers; i++)
	{
		xPos = data.users[i].playa.getposition('x');
		yPos = data.users[i].playa.getposition('y');
		zPos = data.users[i].playa.getposition('z');
		playerPos = DirectX::XMFLOAT3(xPos, yPos, zPos);
		subtractionXMFLOAT3(playerPos, kingOfTheHillOrigo);
		if (getLength(playerPos) <= radius)
		{
			//std::cout << "Innanf�r zonen\n";
			if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - timer)).count() > time)
			{
				if (i == 0 || i == 1)
				{
					team1Score += pointsToAdd;
					std::cout << "team1 got points, total points: " << team1Score << "\n";
				}
				else if (i == 2 || i == 3)
				{
					team2Score += pointsToAdd;
					std::cout << "team2 got points, total points: " << team2Score << "\n";
				}
				timer = std::chrono::system_clock::now();
			}

		}
		else
		{
			//std::cout << "utanf�r zonen\n";
		}
	}

	if (team1Score >= goalScore)
	{
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - this->timerToSend)).count() > timerSend)
		{
			for (int i = 0; i < this->nrOfPlayers; i++)
			{
				if (i == 0 || i == 1)
				{
					winner win;
					win.packetId = PacketType::WINNER;
					sendBinaryDataAllPlayers<winner>(win, data);
					this->timerToSend = std::chrono::system_clock::now();
				}
				else if (i == 2 || i == 3)
				{

				}
			}
		}
	}

}