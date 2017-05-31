#include "LXSpeedWeapon.h"
#include "Action/CCActionIntervalEx.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"

LXSpeedWeapon::LXSpeedWeapon()
	: LXWeapon()
{
}

LXSpeedWeapon::~LXSpeedWeapon() 
{
}

void LXSpeedWeapon::Init(int value)
{
	LXWeapon::Init(value);

}

void LXSpeedWeapon::Tick(float fDelta)
{
	LXWeapon::Tick(fDelta);

}

void LXSpeedWeapon::Destroy()
{
	LXWeapon::Destroy();
}

void LXSpeedWeapon::InitAction()
{
	// 创建子弹移动动作
	LXShooter *pShooter = m_pShooterLogic->GetShooter();
	if (pShooter)
	{
		LXMoveBy *pAction = LXMoveBy::create(GetBulletSpeed(), 
			GetBulletMaxSpeed(),
			GetBulletMinSpeed(),
			GetBulletDegree(),
			GetBulletDeltaSpeed(),
			0.0f,
			true,
			GetBulletSpeedChange());

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

void LXSpeedWeapon::SetShooterLogic(LXShooterLogic* pLogic)
{
	LXWeapon::SetShooterLogic(pLogic);

	// 设置子弹信息
	SetBulletDeltaSpeed(pLogic->GetShooter()->GetSpeedDelta());
	SetBulletMinSpeed(pLogic->GetShooter()->GetMinSpeed());
	SetBulletMaxSpeed(pLogic->GetShooter()->GetMaxSpeed());
	SetBulletSpeedChange(pLogic->GetShooter()->GetDeltaSpeedChange());
}