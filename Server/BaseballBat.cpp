#include "BaseballBat.h"
#include "ObjectId.h"

using namespace DirectX;

BaseballBat::BaseballBat(const int& onlineId)
	:Item(onlineId,ObjID::BAT), force(0.f)
{
	force = FORCECONSTANT;
}

BaseballBat::~BaseballBat()
{
}

void BaseballBat::use(const Player* playerHoldingItem)
{
	std::cout << "use(const Player* playerHoldingItem) on server isnt in use!\n";
}
