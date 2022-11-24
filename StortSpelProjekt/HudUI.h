
#include "GUISprite.h"
#include "GUIText.h"
#include "Player.h"
#include "TimeStruct.h"
class SpaceShip;

class HudUI final
{
	GUISprite fade;


	GUISprite landing0, landing1, landing2;

	GUISprite redTeam0, redTeam1, redTeam2, redTeam3, redTeam4;

	GUISprite blueTeam0,blueTeam1,blueTeam2,blueTeam3,blueTeam4;

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
	float count = 1.0f;
	inline static SpaceShip* red = nullptr;
	inline static SpaceShip* blue = nullptr;
	inline static Player* player = nullptr;

	HudUI();
	~HudUI();
	void handleInputs();
	bool fadeIn();
	void fadeOut();
	void setOpacity(bool onOff);
	void Draw();
};