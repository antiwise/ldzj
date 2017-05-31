#include "LXTrackWeapon.h"
#include "Action/CCActionIntervalEx.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"
#include "GameDefine.h"
#include "ObjectEx/LXObjectManager.h"

LXTrackWeapon::LXTrackWeapon()
	: LXWeapon(),
	m_pTrackAction(nullptr)
{
}

LXTrackWeapon::~LXTrackWeapon() 
{
}

void LXTrackWeapon::Init(int value)
{
	LXWeapon::Init(value);

}

void LXTrackWeapon::Tick(float fDelta)
{
	if(GetResID() == 18) 
	{
		// ����������Чλ����Ϣ
	}

	LXWeapon::Tick(fDelta);
}

void LXTrackWeapon::Destroy()
{
	LXWeapon::Destroy();
}


void LXTrackWeapon::InitAction()
{
	// �����ӵ��ƶ�����
	// ��ȡĿ�����
	LXObject *pTarget = ObjectManager().GetRandomObject(!IsPlayer());
	// ������ܻ���λ�����⣿��
	int resID = GetResID();
	if (resID != 18 && resID != 33 && resID != 78)
	{
		m_pTrackAction = LXMoveTrack::create(GetBulletSpeed(), 
			GetBulletMaxSpeed(),
			GetBulletMinSpeed(),
			GetBulletDegree(),
			0.0f,
			0.0f,
			false,
			false,
			GetBulletDelta(),
			GetBulletMaxDeltaDegree(),
			pTarget,
			0.0f,
			0.0f);

	}
	else
	{
		auto glview = Director::getInstance()->getOpenGLView();
		const auto screenSize = glview->getDesignResolutionSize();
		m_pTrackAction = LXMoveTrack::create(GetBulletSpeed(), 
			GetBulletMaxSpeed(),
			GetBulletMinSpeed(),
			GetBulletDegree(),
			0.0f,
			0.0f,
			false,
			false,
			GetBulletDelta(),
			GetBulletMaxDeltaDegree(),
			pTarget,
			getPosition().x,
			screenSize.height + 100);
	}

	runAction(m_pTrackAction);
}

void LXTrackWeapon::SetShooterLogic(LXShooterLogic* pLogic)
{
	LXWeapon::SetShooterLogic(pLogic);

	// �����ӵ���Ϣ
	SetBulletMaxDeltaDegree(pLogic->GetShooter()->GetMaxDeltaDegree());
}
