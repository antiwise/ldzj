#include "LXParentWeapon.h"

LXParentWeapon::LXParentWeapon()
	: LXWeapon()
{
}

LXParentWeapon::~LXParentWeapon() 
{
}

void LXParentWeapon::Init(int value)
{
	LXWeapon::Init(value);

}

void LXParentWeapon::Tick(float fDelta)
{
	LXWeapon::Tick(fDelta);

}

void LXParentWeapon::Destroy()
{
	LXWeapon::Destroy();
}
