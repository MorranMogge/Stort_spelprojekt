
#include "GUISprite.h"
#include "GUIText.h"
#include "Player.h"
#include "TimeStruct.h"
class SpaceShip;

class HudUI final
{
	GUISprite fade;


	GUISprite landing0;
	GUISprite landing1;
	GUISprite landing2;

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

	GUISprite control;
	GUISprite useText;
	GUISprite throwText;
	GUISprite pickText;
	GUISprite objective;
	GUISprite blackBackground;

	GUISprite controls;
	GUISprite useControls;
	GUISprite useControls1;

	TimeStruct rocketTimer;
	TimeStruct timer;
	float landingCounter;

	void SpritePass();

public:

	inline static SpaceShip* red = nullptr;
	inline static SpaceShip* blue = nullptr;
	inline static Player* player = nullptr;

	HudUI();
	~HudUI();
	void handleInputs();
	void Draw();
};