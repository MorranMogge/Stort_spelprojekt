
#include "GUISprite.h"
#include "GUIText.h"
#include "Player.h"
#include "TimeStruct.h"
class SpaceShip;

class HudUI final
{
	GUISprite fade;

	GUISprite redTeam0, redTeam1, redTeam2, redTeam3, redTeam4;

	GUISprite blueTeam0,blueTeam1,blueTeam2,blueTeam3,blueTeam4;

	GUISprite ready, set, go; 
	GUISprite dot1, dot2, exclamation;


	GUISprite control, control2;
	GUISprite useText;
	GUISprite throwText;
	GUISprite pickText;
	GUISprite objective;
	GUISprite blackBackground;

	GUISprite objective2;
	GUISprite blackBackground2;
	float objectiveDisplayTime = 5.0f;

	GUISprite controls, controls2;
	GUISprite useControls;
	GUISprite useControls1;
	GUISprite useControls2;

	TimeStruct rocketTimer;

	float scaleTimer;
	bool changedSprite = false;
	void SpritePass();
	bool Bready = false;
	bool Bset = false;
	bool Bgo = false;
	bool Bdot1 = false;
	bool Bdot2 = false;
	bool handle = false;

	inline static DirectX::GamePad* gamepad = nullptr;
	inline static DirectX::GamePad::State state;

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
	void moveSprite();
	bool redySetGo();
	void setOpacity(bool onOff);
	static void SetGamePad(DirectX::GamePad* gamepad);
	void Draw();
};