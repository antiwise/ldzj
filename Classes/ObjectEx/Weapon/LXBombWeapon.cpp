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
	
	//  �ο� b.a.d.a 

}

void LXBombWeapon::Tick(float fDelta)
{
	LXWeapon::Tick(fDelta);

}

void LXBombWeapon::Destroy()
{
	LXWeapon::Destroy();
}
