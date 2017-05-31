#include "LXBombWeapon.h"

LXBombWeapon::LXBombWeapon()
	: LXWeapon()
{
}

LXBombWeapon::~LXBombWeapon() 
{
}

void LXBombWeapon::Init(int value)
{
	LXWeapon::Init(value);
	
	//  ²Î¿¼ b.a.d.a 

}

void LXBombWeapon::Tick(float fDelta)
{
	LXWeapon::Tick(fDelta);

}

void LXBombWeapon::Destroy()
{
	LXWeapon::Destroy();
}
