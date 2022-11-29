#include "KingOfTheHillMiniGame.h"

KingOfTheHillMiniGame::KingOfTheHillMiniGame(const short& nrOfPlayers)
	:kingOfTheHillOrigo(0,40,-40), radius(30), nrOfPlayers(nrOfPlayers), team1Score(0), team2Score(0), pointsToAdd(10), time(5), goalScore(100), redInside(false), blueInside(false), oneInside(false)
{
	this->timer = std::chrono::system_clock::now();
	this->timerToSend = std::chrono::system_clock::now();
	timerSend =  4.0f;
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

	oneInside = false;
	blueInside = false;
	redInside = false;

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
			oneInside = true;

			//std::cout << "Innanf�r zonen\n";
			if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - timer)).count() > time)
			{
				if (i == 0 || i == 1)
				{
					blueInside = true;
					team1Score += pointsToAdd;
					std::cout << "team1 got points, total points: " << team1Score << "\n";
				}
				else if (i == 2 || i == 3)
				{
					redInside = true;
					team2Score += pointsToAdd;
					std::cout << "team2 got points, total points: " << team2Score << "\n";
				}

				//Sending points to players
				KTHPoints sendPoints;
				sendPoints.packetId = PacketType::KTHPOINTS;
				sendPoints.bluePoints = team1Score;
				sendPoints.redPoints = team2Score;
				if (blueInside && redInside) sendPoints.teamColor = 0;
				else if (blueInside) sendPoints.teamColor = 1;
				else if (redInside) sendPoints.teamColor = 2;
				sendBinaryDataAllPlayers<KTHPoints>(sendPoints, data);
				timer = std::chrono::system_clock::now();
			}
		}
	}

	//Restoring color
	if (!oneInside)
	{
		ZoneColor color;
		color.packetId = PacketType::ZONECOLOR;
		sendBinaryDataAllPlayers<ZoneColor>(color, data);
		timer = std::chrono::system_clock::now();
	}
	
	//Checks is anyone has won
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
					sendBinaryDataOnePlayer<winner>(win, data.users[i]);
					//sendBinaryDataAllPlayers(win, data);
					//this->timerToSend = std::chrono::system_clock::now();
				}
				else if (i == 2 || i == 3)
				{
					Loser lose;
					lose.packetId = PacketType::LOSER;
					sendBinaryDataOnePlayer<Loser>(lose, data.users[i]);
					//this->timerToSend = std::chrono::system_clock::now();
				}
			}
			this->timerToSend = std::chrono::system_clock::now();
		}
	}

	if (team2Score >= goalScore)
	{
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - this->timerToSend)).count() > timerSend)
		{
			for (int i = 0; i < this->nrOfPlayers; i++)
			{
				if (i == 0 || i == 1)
				{
					Loser lose;
					lose.packetId = PacketType::LOSER;
					sendBinaryDataOnePlayer<Loser>(lose, data.users[i]);
					
				}
				else if (i == 2 || i == 3)
				{
					winner win;
					win.packetId = PacketType::WINNER;
					sendBinaryDataOnePlayer<winner>(win, data.users[i]);
				}
			}
			this->timerToSend = std::chrono::system_clock::now();
		}
	}
	//fixa f�r team 2
}