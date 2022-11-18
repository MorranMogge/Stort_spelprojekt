#pragma once
#include <string>
#include "Item.h"

//temp klass eftersom det inte finns en player
class Component : public Item
{
private:


public:
	Component();
	Component(int onlineId, int onlineType);

	virtual void use() override;
};