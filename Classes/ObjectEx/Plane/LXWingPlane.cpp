#include "LXWingPlane.h"
#include "Animation/AnimationLogic.h"
#include "ObjectEx/LXObjectManager.h"
#include "Common/LXUtils.h"
#include "Shooter/LXShooterManager.h"
#include "GameDefine.h"
#include "DB/DBManager.h"

LXWingPlane::LXWingPlane()
	: LXPlane()
{
	m_bIsPlayer = true;
}

LXWingPlane::~LXWingPlane() 
{
}

void LXWingPlane::Init(int value)
{
	LXObject::Init(value);
}

void LXWingPlane::Tick(float fDelta)
{
	LXPlane::Tick(fDelta);

	
}

void LXWingPlane::Destroy()
{
	LXPlane::Destroy();
}

void LXWingPlane::Init(int resID, bool bFlip)
{
	Init(resID);

	const int nNum = resID + 1;

	//设置攻击力
	this->SetAttack(DBManager().GetWingAttack());

	// 初始化渲染
	std::string strAnimationName = "wing" + StringUtils::toString(nNum);// StringUtils::toString(nNum);
	PlayAnimaton(strAnimationName, g_AnimationName_g0, "", animation_type_repeat, bFlip);

	// 初始化弹幕信息
	std::string shotername;
	if (nNum !=1 && nNum!=5)
	{
		shotername = "liao" + StringUtils::toString(nNum);
	}
	else
	{
		if (bFlip)
		{
			shotername = "liao" + StringUtils::toString(nNum) + "zuo";
		}
		else
		{
			shotername = "liao" + StringUtils::toString(nNum) + "you";
		}
	}

	LXShooterGroup *pShooters = ShooterManager().GetShooterByName(shotername);
	if (pShooters)
	{
		InitShooters(pShooters);
	}

	
	//InitShootLogicByIndex();

	/*
	int var3 = var1 + 1;
	this.h(l.n());					// 攻击力？？
	this.a("wing" + var3, "g_0");		// 播放动画
	String var6;
	if(var3 != 1 && var3 != 5) {
	var6 = "liao" + var3;
	} else {
	StringBuilder var4 = (new StringBuilder("liao")).append(var3);
	String var5;
	if(var2) {
	var5 = "zuo";
	} else {
	var5 = "you";
	}

	var6 = var4.append(var5).toString();//
	}

	this.a(n.a(var6));//初始化弹幕
	this.a(var2);

	for(int var7 = 0; var7 < this.l.size; ++var7) {
	((com.sg.raiden.b.a.a.d)this.l.get(var7)).b(-20.0F);
	}
	*/

	////// 允许发射子弹	当前血量大于0
	//if(m_bAutoShoot && IsAlive())
	//{
	//	InitShootLogicByIndex(m_nWeaponInfo);
	//}

	//for (int i = 0; i < m_WingPlanes.size(); i++)
	//{
	//	m_WingPlanes[i]->InitShootLogic();
	//}

	// 数据初始化
	InitData();
}

void LXWingPlane::InitData()
{

}