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

	// ���ǳ�ʼ��
	InitData();

	// ��ʼ����Ⱦ����
	std::string path = gPlayerInfo[value];
	PlayAnimaton( path, g_AnimationName_g3 );

	// ��ʼ����Ļ
	initShooter();

	// ��ʼ������߼�
	//InitShootLogic();

	// ʹ�û���
	if ( !IsUIObject() )
	{
		CreatProtectEffect();
	}
	
}

void LXPlayerPlane::InitData()
{
	// λ��
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

	// Ѫ��
	m_nAttack = DBManager().GetPlaneAttack();
	m_nCurrentHP = 100;
	m_nTotalHP = 100;
	// �Զ����
	m_bAutoShoot = true;

	// ���ɰ�Χ��
	DBC_DEFINEHANDLE(s_pUserDBC, DBC_USER);
	const _DBC_USER *pUserData = (const _DBC_USER*)s_pUserDBC->Search_LineNum_EQU(calcUserDataID());
	if (pUserData)
	{
		SetCollideInfo(Rect(pUserData->nRect1, pUserData->nRect2, pUserData->nRect3, pUserData->nRect4));

		// ������������
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

	// �ӵ�����
	if (m_bAutoShoot)
	{
		// ����״̬ʱ��
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

	// �����Ż�
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

	// �޵�ʱ�䵭�뵭��Ч��
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

	// ���ڻ���
	if(m_fProtected > 0)
	{
		m_fProtected -= fDelta;

		// ���з���������ӵ�ȫ����Ϊˮ��
		ObjectManager().EnemyWeaponBecomeCrystal();
		
		if (m_fProtected <=0)
		{
			m_fProtected = 0;
			// ��ʾ����

		}
	}

	if ( m_isRenascenceing )
	{
		m_RenascenceTime -= fDelta;
		if( m_RenascenceTime <= 0.0F )
		{
			m_IsAutoMove = false;
			m_isRenascenceing = false;
			InitShootLogic();				// ��ʼ������߼�
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

	// ս�����
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
	// ʰȡ����
	if (m_nCurrentHP > 0 && isVisible())
	{
		// ��ȡ������ƷID
		int nResID = pObject->GetResID();

		switch (nResID)
		{
		case 1:		//�������� huoli02
			{
				if ( m_nLevel == 4 )
				{
					m_fCrazyTime = DBManager().GetCrazyTime();		// ���� ���� ʱ��
				}else if( m_nLevel + 1 == 4 )
				{
					SetLevel( m_nLevel + 1 );
					m_fCrazyTime = DBManager().GetCrazyTime();		// ���� ���� ʱ��
					SetCrazyState(true);							// ���ñ���״̬
	
					Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
					strName = "p0/powermax.p";
					EffectManager().CreateEffect(pParent, strName , Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);

					//  ������Ч
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
					SetWeaponSpecialEffect();			// ��������������Ч	
					InitShootLogic();					// ��ʼ������߼�
					// w.c("powerup.ogg");												// ������Ч
					Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
					strName = "p0/powerup.p";
					EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
					SoundSystem().playEffect(Mic_powerup);
				}
			}
			break;
		case 2:			//�������� huoli03
			{
				if ( m_nLevel < 3 )
				{
					SetLevel(3);
					SetWeaponSpecialEffect();			// ��������������Ч	
					InitShootLogic();					// ��ʼ������߼�
				}else if( m_nLevel == 3 )
				{
					SetLevel(4);
					m_fCrazyTime = DBManager().GetCrazyTime();		// ���� ���� ʱ��
					SetCrazyState(true);							// ���ñ���״̬
					// w.c("levelmax" + MathUtils.random(1, 2) + ".ogg");						// ������Ч
					// w.c("powerup.ogg");														// ������Ч
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
					m_fCrazyTime = DBManager().GetCrazyTime();		// ���� ���� ʱ��
				}
			}
			break;
		case 3:		//�������� zhuanhuan05
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
		case 4:		//����Ѫ������ jiaxie04
			{
				// w.c("life.ogg");
				Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
				strName = "p0/lifeup.p";
				EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(Mic_life);
				DBManager().AddMissionLife();
			}
			break;
		case 5:		//ը��(����)�ɻ� baoxian01
			{
				// w.c("bomb.ogg");
				Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
				strName = "p0/bombup.p";
				EffectManager().CreateEffect(pParent, strName, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(Mic_bomb);
				DBManager().AddMissionBomb();
			}
			break;
		case 6:			// ˮ��
		case 7:
		case 8:
			{
				//SoundSystem().playEffect(Mic_crystal);
				DBManager().AddDiamondByCrystal( nResID - 5 );
			}
			break;
		case 9:			// ˮ��
		case 10:
		case 11:
			{
				//SoundSystem().playEffect(Mic_crystal);
			}
			break;
		case 12:		// ˮ��
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
	// ��ʼ����ҵ�Ļ
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
		// ����ʱ���������ӵ�
		return ;
	}

	//
	if (m_nShooterEffect ==0)
	{
		SoundSystem().playEffect(Mic_bullet1);
		m_nShooterEffect = 1;
	}

	// �������ӵ�	��ǰѪ������0
	if( m_bAutoShoot && IsAlive() )
	{
		// ���¼��㵯Ļ��Ϣ
		m_nWeaponInfo.clear();
		DBC_DEFINEHANDLE(s_pUserDBC, DBC_USER);
		const _DBC_USER *pUserData = (const _DBC_USER*)s_pUserDBC->Search_LineNum_EQU(calcUserDataID());
		if (pUserData)
		{
			// ������������
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
	//this.q();			// �ӵ���Ϣ����
// 	switch(m_PlaneType) 
// 	{			// ս������
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

	// ����Ļ
	StopShootLogic();
}

void LXPlayerPlane::StopShootLogic()
{
	if (m_nShooterEffect)
	{
		SoundSystem().stopEffect(m_nShooterEffect);
		m_nShooterEffect = 0;
	}

	// ��������Ļ
	LXPlane::StopShootLogic();

	// �����Ż���Ļ
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

	SetWeaponSpecialEffect();			// ��������������Ч	
	InitShootLogic();					// ��ʼ������߼�

	m_bCrazyMode = isCrazy;
	std::string strName;
	if( isCrazy )
	{
		if( m_PlaneType != 3 )
		{
			PlayAnimaton(m_Path, g_AnimationName_g13);		// ���ű�����
			SetNextAnimation( g_AnimationName_g8 );			// ������һ��Ҫ���ŵĶ���
		}

		strName = "p0/transform.p";
		EffectManager().CreateEffect(this, strName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false);
		SoundSystem().playEffect(Mic_transform);
		std::string var5 = "p1/transforming" + ( 1 + m_PlaneType );
		if(m_PlaneType == 4)
		{
			// Y = b("transform5_open", this.W);	// ����һ�����ӡ����浽Y
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
	if ( DBManager().GetMissionShield() > 0 && m_ProtectEffect == nullptr)		// �������������㣬����������ЧxΪ��
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

// ������Ļ
void LXPlayerPlane::RunIntoScreen()
{
	SoundSystem().playEffect(Mic_entry);
	m_isRenascenceing = true;
	m_IsAutoMove = true;

	ShowPlane(true);
	SetHP(100);
	SetLevel(DBManager().GetPlaneWeaponLevel());	// ���÷ɻ��ȼ�Ϊ�����ȼ�
	// ����Ҫ�������ɰ�Χ�У�����
	//InitShootLogic();
	SetWeaponSpecialEffect();
	PlayAnimaton(m_Path, g_AnimationName_g3);
	SetNextAnimation( g_AnimationNameNull );
	m_bCrazyMode = false;
	m_fCrazyTime = 0.0f;

// 	if(this.Y != null) {          // ����
// 		this.Y.c();
// 		this.Y = null;
// 	}
// 
// 	if(this.Z != null) {
// 		this.Z.c();
// 		this.Z = null;
// 	}

	this->setPosition( Utils().GetFixedX(240), -100.0f);

	// ����һ������
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
	
	// ֹͣ��Ļ
	StopShootLogic();
	m_IsAutoMove = true;

	ActionInterval*		_moveTo		= MoveTo::create( 1.5f, Vec2( this->getPositionX(), screenSize.height + 500 ) );
	CallFunc*			_callFunc	= CallFunc::create( CC_CALLBACK_0(LXPlayerPlane::FighterWinCallback,this) );			// ����2������ص�
	FiniteTimeAction*	_action		= Sequence::create( _moveTo, _callFunc, NULL );
	
	this->runAction(_action);

}

void LXPlayerPlane::FighterWinAndChargeCallback()
{

}

void LXPlayerPlane::FighterWinCallback()
{
	// ���ص��ɻ�
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
		
	// ������ɱ
    m_fBombTime = 0.0F;					// ����ʱ��
	SetInvincibleTime(3);				// �����޵�ʱ��3��
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
			float time = y / 300.0F;	// �ƶ�ʱ��
			// ����
			pObject->runAction(cocos2d::EaseQuadraticActionIn::create(CCMoveBy::create(time, Vec2(0, y))));

			// ��ʼ����ĻЧ��
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
			float time = y / 300.0F;	// �ƶ�ʱ��
			// ����
			pObject->runAction(cocos2d::EaseQuadraticActionIn::create(CCMoveBy::create(time, Vec2(0, y))));

			// ��ʼ����ĻЧ��
			pObject->InitShootLogic();
			dynamic_cast<LXBombPlane*>(pObject)->setBombStatus(false);
		}
	}
	 
}