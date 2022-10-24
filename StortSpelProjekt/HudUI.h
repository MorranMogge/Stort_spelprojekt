
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include "SpaceShip.h"

class HudUI final
{

	GUISprite redTeam0;
	GUISprite redTeam1;
	GUISprite redTeam2;
	GUISprite redTeam3;
	GUISprite redTeam4;

	GUISprite blueTeam0;
	GUISprite blueTeam1;
	GUISprite blueTeam2;
	GUISprite blueTeam3;
	GUISprite blueTeam4;

	GAMESTATE gameState;

	void SpritePass();


public:

	GAMESTATE GetGameState();

	HudUI();
	~HudUI();

	void Draw();
};