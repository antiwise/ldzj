#include "LXPlayerPlane.h"
#include "DataBase/DataBase.h"
#include "LXWingPlane.h"
#include "ObjectEx/LXObjectManager.h"
#include "Common/LXUtils.h"
#include "Shooter/LXShooterManager.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "Effect/ParticleEffect.h"
#include "Effect/LXEffectManager.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "Animation/AnimationLogic.h"
#include "Scene/GameScene.h"
#include "Sound/LXSoundSystem.h"
#include "LXBombPlane.h"
#include "deprecated/CCString.h"

const int BombData1[] = {0, 1, 2, 3, 5};
const int BombData2[] = {4, 6, 7};
const Vec2 BombPos[] = { Vec2(0, 0), Vec2(-85, -60), Vec2(85, -60), Vec2(-170, -120), Vec2(0, -120), Vec2(170, -120), Vec2(-85, -180), Vec2(85, -180)};

LXPlayerPlane::LXPlayerPlane()
	: LXPlane(),
	m_bAutoShoot(true),
	m_isRenascenceing(false),
	m_nLevel(0),
	m_nWingPosParam1(0),
	m_nWingPosParam2(0),
	m_fWingPosParam3(0.0f),
	m_fWingPosParam4(0.0f),
	m_bCrazyMode(false),
	m_fInvincible(5.0f),
	m_fProtected(0.0f),
	m_PlaneType(0),
	m_fCrazyTime(0.0f),
	m_RenascenceTime(0.0f),
	m_ProtectEffect(nullptr),
	m_IsAutoMove(false),
	m_EffectY(nullptr),
	m_nShooterEffect(0),
	m_bBombActive(false),
	m_fBombTime(0.0f)
{
	for (int i=0; i<10; i++)
	{
		m_WingTempXPos[i] = 0;
		m_WingTempYPos[i] = 0;
	}

	m_bIsPlayer = true;
}

LXPlayerPlane::~LXPlayerPlane() 
{
}

void LXPlayerPlane::Init(int value)
{
	LXObject::Init(value);

	// 主角初始化
	InitData();

	// 初始化渲染数据
	std::string path = gPlayerInfo[value];
	PlayAnimaton( path, g_AnimationName_g3 );

	// 初始化弹幕
	initShooter();

	// 初始化射击逻辑
	//InitShootLogic();

	// 使用护盾
	if ( !IsUIObject() )
	{
		CreatProtectEffect();
	}
	
}

void LXPlayerPlane::InitData()
{
	// 位置
	setPosition( Size( Utils().GetFixedX(240), 200));

	m_nWingPosParam1 = 0;
	m_nWingPosParam2 = 0;
	m_fWingPosParam3 = Utils().GetFixedX(240);
	m_fWingPosParam4 = 200;
	for (int i=0; i<10; i++)
	{
		m_WingTempXPos[i] = 0;
		m_WingTempYPos[i] = 0;
	}
	m_nWeaponInfo.clear();

	// 血量
	m_nAttack = DBManager().GetPlaneAttack();
	m_nCurrentHP = 100;
	m_nTotalHP = 100;
	// 自动设计
	m_bAutoShoot = true;

	// 生成包围盒
	DBC_DEFINEHANDLE(s_pUserDBC, DBC_USER);
	const _DBC_USER *pUserData = (const _DBC_USER*)s_pUserDBC->Search_LineNum_EQU(calcUserDataID());
	if (pUserData)
	{
		SetCollideInfo(Rect(pUserData->nRect1, pUserData->nRect2, pUserData->nRect3, pUserData->nRect4));

		// 计算武器数据
		if (pUserData->szShooter)
		{
			std::string data = std::string(pUserData->szShooter);
			data = data.substr(1, data.length() - 2);
			std::vector<std::string> shooterVec;
			Utils().split(data, shooterVec, ",");
			for (int i = 0; i < shooterVec.size(); i++)
			{
				m_nWeaponInfo.push_back(atoi(shooterVec[i].c_str()));
			}
		}
	}
}

void LXPlayerPlane::Tick(float fDelta)
{
	LXPlane::Tick(fDelta);

	if (IsUIObject())
	{
		return ;
	}
	
	if (!m_bUIObject && ObjectManager().GetPlayer() && ObjectManager().GetPlayer()->IsPause())
	{
		return;
	}

	// 子弹发射
	if (m_bAutoShoot)
	{
		// 暴走状态时间
		if ( m_fCrazyTime > 0.0f )
		{
			m_fCrazyTime -= fDelta;
		}else if ( m_nLevel == 4 )
		{
			SetLevel(3);
			SetCrazyState(false);
		}

		if (m_bBombActive)
		{
			m_fBombTime += fDelta;
			int bombLevel = DBManager().GetData(DATA_BOMB_LEVEL);
			if(m_fBombTime >= 5.0F + 0.7F * bombLevel) 
			{
				 m_bBombActive = false;
			}
		}
	}

	// 设置僚机
	if (m_nWingPosParam1 < 10)
	{
		++m_nWingPosParam1;
	}

	m_WingTempXPos[m_nWingPosParam2] = getPosition().x - m_fWingPosParam3;
	m_WingTempYPos[m_nWingPosParam2] = getPosition().y - m_fWingPosParam4;
	m_fWingPosParam3 = getPosition().x;
	m_fWingPosParam4 = getPosition().y;

	m_nWingPosParam2 = (m_nWingPosParam2 + 1) % 10;

	for(int i = 0; i < m_WingPlanes.size(); i++)
	{
		int index = g_WingPos[i];

		if (index < m_nWingPosParam1)
		{
			int index1 = (10 + m_nWingPosParam2 - index) % 10;
			m_WingPlanes[i]->setPosition(ccpAdd(m_WingPlanes[i]->getPosition(), ccp(m_WingTempXPos[index1], m_WingTempYPos[index1])));
		}
	}

	// 无敌时间淡入淡出效果
	if (m_fInvincible > 0)
	{
		m_fInvincible -= fDelta;
		if (m_fInvincible < 0.0f)
		{
			m_fInvincible = 0.0f;
		}

		float alpha = fabs(float(5 - (int)(20 * m_fInvincible) % 10)) / 10.0f;
		setOpacity(alpha * 255);
	}
	else
	{
		setOpacity(255);
	}

	// 存在护盾
	if(m_fProtected > 0)
	{
		m_fProtected -= fDelta;

		// 所有发射出来的子弹全部变为水晶
		ObjectManager().EnemyWeaponBecomeCrystal();
		
		if (m_fProtected <=0)
		{
			m_fProtected = 0;
			// 显示场景

		}
	}

	if ( m_isRenascenceing )
	{
		m_RenascenceTime -= fDelta;
		if( m_RenascenceTime <= 0.0F )
		{
			m_IsAutoMove = false;
			m_isRenascenceing = false;
			InitShootLogic();				// 初始化射击逻辑
			CreatProtectEffect();
		}
	}

}

void LXPlayerPlane::Destroy()
{
	LXPlane::Destroy();

	m_WingPlanes.clear();
	m_nWeaponInfo.clear();
	m_ProtectEffect = nullptr;
	m_IsAutoMove = false;

}

void LXPlayerPlane::Hit(int damage)
{
	m_nCurrentHP -= damage;

	// 战机损毁
	if (m_nCurrentHP <= 0)
	{
		m_bDeadFlag = true;
	}
}


void LXPlayerPlane::Move(Vec2 delta)
{
	if (!m_IsAutoMove)
	{
		this->setPosition(this->getPosition() + delta);
	}
}

void LXPlayerPlane::PickItem(LXObject *pObject)
{
	if (pObject == NULL)
	{
		return;
	}

	std::string strName = "";
	// 拾取道具
	if (m_nCurrentHP > 0 && isVisible())
	{
		// 获取掉落物品ID
		int nResID = pObject->GetResID();

		switch (nResID)
		{
		case 1:		//武器升级 huoli02
			{
				if ( m_nLevel == 4 )
				{
					m_fCrazyTime = DBManager().GetCrazyTime();		// 重置 暴走 时间
				}else if( m_nLevel + 1 == 4 )
				{
					SetLevel( m_nLevel + 1 );
					m_fCrazyTime = DBManager().GetCrazyTime();		// 重置 暴走 时间
					SetCrazyState(true);							// 设置暴走状态
	
					Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
					strName = "p0/powermax.p";
					EffectManager().CreateEffect(pParent, strName , Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);

					//  播放音效
					const int num = std::rand()%2;
					if ( m_PlaneType != 4 )
					{
						if (num == 0)
						{
							SoundSystem().playEffect(Mic_levelmax1);
						}
						else
						{
							SoundSystem().playEffect(Mic_levelmax2);
						}
						//w.c("levelmax" + MathUtils.random(1, 2) + ".ogg");
					}else
					{
						if (num == 0)
						{
							SoundSystem().playEffect(Mic_bs1);
						}
						else
						{
							SoundSystem().playEffect(Mic_bs2);
						}
						//w.c("bs" + MathUtils.random(1, 2) + ".ogg");
					}

				}else
				{
					SetLevel( m_nLevel + 1 );
					SetWeaponSpecialEffect();			// 设置武器发射特效	
					InitShootLogic();					// 初始化射击逻辑
					// w.c("powerup.ogg");												// 播放音效
					Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
					strName = "p0/powerup.p";
					EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
					SoundSystem().playEffect(Mic_powerup);
				}
			}
			break;
		case 2:			//武器升级 huoli03
			{
				if ( m_nLevel < 3 )
				{
					SetLevel(3);
					SetWeaponSpecialEffect();			// 设置武器发射特效	
					InitShootLogic();					// 初始化射击逻辑
				}else if( m_nLevel == 3 )
				{
					SetLevel(4);
					m_fCrazyTime = DBManager().GetCrazyTime();		// 重置 暴走 时间
					SetCrazyState(true);							// 设置暴走状态
					// w.c("levelmax" + MathUtils.random(1, 2) + ".ogg");						// 播放音效
					// w.c("powerup.ogg");														// 播放音效
					Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
					strName = "p0/powermax.p";
					EffectManager().CreateEffect(pParent, strName, Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);
					const int num = rand()%2;
					if (num == 0)
					{
						SoundSystem().playEffect(Mic_levelmax1);
					}
					else
					{
						SoundSystem().playEffect(Mic_levelmax2);
					}
					
					SoundSystem().playEffect(Mic_powerup);
				}else
				{
					m_fCrazyTime = DBManager().GetCrazyTime();		// 重置 暴走 时间
				}
			}
			break;
		case 3:		//护盾增加 zhuanhuan05
			{
				// w.c("shield.ogg");
				Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
				strName = "p0/shieldup.p";
				EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(Mic_shield);
				DBManager().AddMissionShield();
				CreatProtectEffect();
			} 
			break;
		case 4:		//生命血量增加 jiaxie04
			{
				// w.c("life.ogg");
				Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
				strName = "p0/lifeup.p";
				EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(Mic_life);
				DBManager().AddMissionLife();
			}
			break;
		case 5:		//炸弹(保险)飞机 baoxian01
			{
				// w.c("bomb.ogg");
				Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
				strName = "p0/bombup.p";
				EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(Mic_bomb);
				DBManager().AddMissionBomb();
			}
			break;
		case 6:			// 水晶
		case 7:
		case 8:
			{
				//SoundSystem().playEffect(Mic_crystal);
				DBManager().AddDiamondByCrystal( nResID - 5 );
			}
			break;
		case 9:			// 水晶
		case 10:
		case 11:
			{
				//SoundSystem().playEffect(Mic_crystal);
			}
			break;
		case 12:		// 水晶
			{
				//SoundSystem().playEffect(Mic_crystal);
				DBManager().AddDiamondByCrystal( 10 );
			}
			break;
		default:
			break;
		}

		if ( nResID >= 6 && nResID <= 12 )
		{
			strName = "p0/getcrystal.p";
			EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false);
		}else
		{
			strName = "p0/goods.p";
			EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false);
		}

	}
}

int	LXPlayerPlane::calcUserDataID()
{
	return m_nResID*5 + m_nLevel;
}

void LXPlayerPlane::AddWingPlane(LXWingPlane* pWing)
{
	if (pWing)
	{
		m_WingPlanes.push_back(pWing);
	}
}

void LXPlayerPlane::initShooter()
{
	// 初始化玩家弹幕
	std::string strShooterName = "user" + StringUtils::toString(m_nResID + 1);// StringUtils::toString(m_nResID + 1);
	LXShooterGroup *pShooters = ShooterManager().GetShooterByName(strShooterName);
	if (pShooters)
	{
		InitShooters(pShooters);
	}
}

void LXPlayerPlane::InitShootLogic()
{
	if (m_isRenascenceing)
	{
		// 重生时不允许发射子弹
		return ;
	}

	//
	if (m_nShooterEffect ==0)
	{
		SoundSystem().playEffect(Mic_bullet1);
		m_nShooterEffect = 1;
	}

	// 允许发射子弹	当前血量大于0
	if( m_bAutoShoot && IsAlive() )
	{
		// 重新计算弹幕信息
		m_nWeaponInfo.clear();
		DBC_DEFINEHANDLE(s_pUserDBC, DBC_USER);
		const _DBC_USER *pUserData = (const _DBC_USER*)s_pUserDBC->Search_LineNum_EQU(calcUserDataID());
		if (pUserData)
		{
			// 计算武器数据
			if (pUserData->szShooter && strlen(pUserData->szShooter) > 0)
			{
				std::string data = pUserData->szShooter;
				data = data.substr(1, data.length() - 2);
				std::vector<std::string> shooterVec;
				Utils().split(data, shooterVec, ",");
				for (int i = 0; i < shooterVec.size(); i++)
				{
					m_nWeaponInfo.push_back(atoi(shooterVec[i].c_str()));
				}
			}
		}

		InitShootLogicByIndex(m_nWeaponInfo);

		for (int i = 0; i < m_WingPlanes.size(); i++)
		{
			m_WingPlanes[i]->StopShootLogic();
			m_WingPlanes[i]->InitShootLogic();
		}
	}
}

void LXPlayerPlane::SetWeaponSpecialEffect()
{
	StopShootLogic();
	//this.q();			// 子弹信息清理
// 	switch(m_PlaneType) 
// 	{			// 战机类型
// 	case 0:
// 		int[] var2 = new int[]{this.O[0]};
// 		this.a("user_fire", var2);
// 		return;
// 	case 1:
// 		int[] var1 = new int[]{this.O[0]};
// 		this.a("user_firedian", var1);
// 		return;
// 	default:
// 	}

}

void LXPlayerPlane::SetFireState(bool value)
{
	m_bAutoShoot = value; 

	// 清理弹幕
	StopShootLogic();
}

void LXPlayerPlane::StopShootLogic()
{
	if (m_nShooterEffect)
	{
		SoundSystem().stopEffect(m_nShooterEffect);
		m_nShooterEffect = 0;
	}

	// 清理自身弹幕
	LXPlane::StopShootLogic();

	// 清理僚机弹幕
	for (int i = 0; i < m_WingPlanes.size(); i++)
	{
		m_WingPlanes[i]->StopShootLogic();
	}
}

void LXPlayerPlane::SetLevel(int value)
{
	m_nLevel = value;
	DBManager().SetPlaneLevel(value);
}


void LXPlayerPlane::SetPlaneType(int type)
{
	m_PlaneType = type;
	DBManager().SetCurrPlaneID(type);
}

void LXPlayerPlane::SetCrazyState(bool isCrazy)
{

	SetWeaponSpecialEffect();			// 设置武器发射特效	
	InitShootLogic();					// 初始化射击逻辑

	m_bCrazyMode = isCrazy;
	std::string strName;
	if( isCrazy )
	{
		if( m_PlaneType != 3 )
		{
			PlayAnimaton(m_Path, g_AnimationName_g13);		// 播放变身动作
			SetNextAnimation( g_AnimationName_g8 );			// 设置下一个要播放的动作
		}

		strName = "p0/transform.p";
		EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false);
		SoundSystem().playEffect(Mic_transform);
		std::string var5 = "p1/transforming" + ( 1 + m_PlaneType );
		if(m_PlaneType == 4)
		{
			// Y = b("transform5_open", this.W);	// 创建一个粒子。保存到Y
			// strName="p0/transform5_open.p";
			//m_EffectY = EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, true);
			return;
		}

		//m_EffectY = EffectManager().CreateEffect(this, "var5", 0, 0, GAME_ZORDER_OBJECT_PARTICLE, true);
	
		// Z = b(var5, this.W);
		// Z.setScaleX(-1.0F);
		// Z.a(true);
	}else
	{
		if ( m_PlaneType != 3 )
		{
			PlayAnimaton( m_Path, g_AnimationName_g13,"", animation_type_rrepeat );
			SetNextAnimation( g_AnimationName_g3 );
		}

		//if( m_EffectY != nullptr )
		//{
			//EffectManager().DeleteEffect(m_EffectY);
			//m_EffectY = nullptr;
		//}

		//if( m_PlaneType == 4 )
		//{
		// strName = "p0/transform5_end.p";
		//	m_EffectY = EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false);
		//}

		/*if(this.Z != null)
		{
		this.Z.c();
		this.Z = null;
		return;
		}*/
	}

}

void LXPlayerPlane::CreatProtectEffect()
{
	if ( DBManager().GetMissionShield() > 0 && m_ProtectEffect == nullptr)		// 护罩数量大于零，并且粒子特效x为空
	{
		std::string strName="p0/shield.p";
		m_ProtectEffect = EffectManager().CreateEffect(this, strName, this->getContentSize().width / 2, this->getContentSize().height / 2, GAME_ZORDER_OBJECT_PARTICLE, true);
	}
	else if(DBManager().GetMissionShield() <= 0)
	{
		ClearUpProtectEffect();
		return ;
	}
}

void LXPlayerPlane::ShowProtectEffect()
{
	std::string strName = "p0/useshield.p";
	ParticleEffect *effect = EffectManager().CreateEffect(this, strName, this->getContentSize().width / 2, this->getContentSize().height / 2, GAME_ZORDER_OBJECT_PARTICLE, false);
}

void LXPlayerPlane::ClearUpProtectEffect()
{
	if (m_ProtectEffect)
	{
		EffectManager().DeleteEffect(m_ProtectEffect);
		m_ProtectEffect = nullptr;
	}
}

// 飞入屏幕
void LXPlayerPlane::RunIntoScreen()
{
	SoundSystem().playEffect(Mic_entry);
	m_isRenascenceing = true;
	m_IsAutoMove = true;

	ShowPlane(true);
	SetHP(100);
	SetLevel(DBManager().GetPlaneWeaponLevel());	// 设置飞机等级为武器等级
	// 还需要重新生成包围盒？？？
	//InitShootLogic();
	SetWeaponSpecialEffect();
	PlayAnimaton(m_Path, g_AnimationName_g3);
	SetNextAnimation( g_AnimationNameNull );
	m_bCrazyMode = false;
	m_fCrazyTime = 0.0f;

// 	if(this.Y != null) {          // 粒子
// 		this.Y.c();
// 		this.Y = null;
// 	}
// 
// 	if(this.Z != null) {
// 		this.Z.c();
// 		this.Z = null;
// 	}

	this->setPosition( Utils().GetFixedX(240), -100.0f);

	// 创建一个东西
	ActionInterval* _delay = DelayTime::create(1.0f);
	ActionInterval* _move = MoveBy::create(2.0f,Vec2(0,300));
	FiniteTimeAction* _Action = Sequence::create( _delay, _move, NULL );
	runAction(_Action);

	m_RenascenceTime = 4.0f;
	m_fInvincible = 7.0f;

}

void LXPlayerPlane::RefreshProtectWithReLife()
{
	DBManager().AddMissionShield( DBManager().GetProtectNum() );
	CreatProtectEffect();
}

void LXPlayerPlane::ShowPlane(bool bShow)
{
	this->setVisible(bShow);
	
	if (!bShow)
	{
		StopShootLogic();
	}

	for ( auto it : m_WingPlanes )
	{
		it->setVisible(bShow);
	}
}

void LXPlayerPlane::FighterWinAndCharge()
{
	//int var1 = 240 + (int)(-com.sg.raiden.a.e.j.c.a().getX());
	//this.c(0.0F, 0.0F);
	//p.c(false);
	//this.clearActions();
	//this.addAction(Actions.sequence(Actions.moveTo(this.getX(), -150.0F, 1.0F, Interpolation.pow2In), Actions.delay(2.5F), Actions.moveTo((float)var1, 948.0F), Actions.moveBy(0.0F, -382.0F, 1.0F, Interpolation.pow3Out)));

}

void LXPlayerPlane::FighterWin()
{
	//p.c(false);
	//this.clearActions();
	//this.addAction(Actions.moveTo(this.getX(), -500.0F, 1.5F, Interpolation.pow2In));

	//ActionInterval*		_delay		= DelayTime::create(1.5f);
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();
	
	// 停止弹幕
	StopShootLogic();
	m_IsAutoMove = true;

	ActionInterval*		_moveTo		= MoveTo::create( 1.5f, Vec2( this->getPositionX(), screenSize.height + 500 ) );
	CallFunc*			_callFunc	= CallFunc::create( CC_CALLBACK_0(LXPlayerPlane::FighterWinCallback,this) );			// 精灵2播放完回调
	FiniteTimeAction*	_action		= Sequence::create( _moveTo, _callFunc, NULL );
	
	this->runAction(_action);

}

void LXPlayerPlane::FighterWinAndChargeCallback()
{

}

void LXPlayerPlane::FighterWinCallback()
{
	// 隐藏掉飞机
	ShowPlane(false);
	m_IsAutoMove = true;
}

void LXPlayerPlane::ActiveBombPlane()
{
	if (m_bBombActive)
	{
		return;
	}

	m_bBombActive = true;
		
	// 启动必杀
    m_fBombTime = 0.0F;					// 重置时间
	SetInvincibleTime(3);				// 设置无敌时间3秒
	SoundSystem().playEffect(Mic_boom_user);
	
    float centerPosX = Utils().GetDesignWidth()/2;
	int bombLevel = DBManager().GetData(DATA_BOMB_LEVEL);

	const int BombData1[] = {0, 1, 2, 3, 5};
	const int BombData2[] = {4, 6, 7};

	int posY = -100;
	for (int i = 0; i < 5; i++)
	{
		int resID = BombData1[i];
		LXObject *pObject = ObjectManager().CreateObject(resID, this->getParent(), object_type_plane_bomb, true);
		if (pObject)
		{
			Vec2 offsetPos = BombPos[resID];
			float targetX = centerPosX + offsetPos.x;
			pObject->setPosition(targetX, posY);
			pObject->SetIsPlayer(true);
			pObject->Init(resID);

			float y = 300 + offsetPos.y - posY;
			float time = y / 300.0F;	// 移动时间
			// 动作
			pObject->runAction(cocos2d::EaseQuadraticActionIn::create(CCMoveBy::create(time, Vec2(0, y))));

			// 初始化弹幕效果
			pObject->InitShootLogic();
			dynamic_cast<LXBombPlane*>(pObject)->setBombStatus(false);
		}
	}
	
	posY -= 120;
	for (int i = 0; i < 3; i++)
	{
		int resID = BombData2[i];
		LXObject *pObject = ObjectManager().CreateObject(resID, this->getParent(), object_type_plane_bomb, true);
		if (pObject)
		{
			Vec2 offsetPos = BombPos[resID];
			float targetX = centerPosX + offsetPos.x;
			pObject->setPosition(targetX, posY);
			pObject->SetIsPlayer(true);
			pObject->Init(resID);

			float y = 300 + offsetPos.y - posY;
			float time = y / 300.0F;	// 移动时间
			// 动作
			pObject->runAction(cocos2d::EaseQuadraticActionIn::create(CCMoveBy::create(time, Vec2(0, y))));

			// 初始化弹幕效果
			pObject->InitShootLogic();
			dynamic_cast<LXBombPlane*>(pObject)->setBombStatus(false);
		}
	}
	 
}