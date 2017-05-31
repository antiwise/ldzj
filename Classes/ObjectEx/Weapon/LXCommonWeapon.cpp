#include "LXCommonWeapon.h"
#include "Action/CCActionIntervalEx.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"

LXCommonWeapon::LXCommonWeapon()
	: LXWeapon()
{
}

LXCommonWeapon::~LXCommonWeapon() 
{
}

void LXCommonWeapon::Init(int value)
{
	LXWeapon::Init(value);
}

void LXCommonWeapon::Tick(float fDelta)
{
	LXWeapon::Tick(fDelta);

}

void LXCommonWeapon::Destroy()
{
	LXWeapon::Destroy();
}

void LXCommonWeapon::InitAction()
{
	LXShooter *pShooter = m_pShooterLogic->GetShooter();
	if (pShooter)
	{
		LXMoveBy *pAction = LXMoveBy::create(GetBulletSpeed(), 
			GetBulletSpeed(), 
			GetBulletSpeed(), 
			GetBulletDegree(),
			0.0f,
			0.0f,
			false,
			false);

		runAction(pAction);

		if (m_bResDir)
		{
			if (IsPlayer())
			{
				setRotation(GetBulletDegree() - 270);
			}
			else
			{
				setRotation(GetBulletDegree() - 90);
			}
		}
		else
		{
			setRotation(0);
		}
	}
}
