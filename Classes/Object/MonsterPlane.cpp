//#include "MonsterPlane.h"
//#include "Object/MonsterBullet.h"
//#include "UI/EnemyLayer.h"
//#include "UI/BulletLayer.h"
//
//MonsterPlane::MonsterPlane(void):Plane()
//{
//	m_nID = 0;
//	m_pMonster = nullptr;
//	m_ePathType = MonsterPathType::LINE;
//	m_fSpeed = 150.0f;
//	m_isAutoHide = true;
//	m_isPlayerAction = false;
//
//	m_AllTime = 0.0f;
//	m_nextFireTiem = 0.0f;
//
//}
//
//
//MonsterPlane::~MonsterPlane(void)
//{
//}
//
//void MonsterPlane::InitObject(std::string texture)
//{
//	m_pMonster = Sprite::create(texture);
//	
//	SetMonsterTagId(EnemyLayer::GetMe()->GetMaxMonsterTagId());
//
//	this->addChild(m_pMonster);
//	
//	EnemyLayer::GetMe()->AddMonsterPlane(this);
//}
//
//void MonsterPlane::InitAndRunAction()
//{
//	// 因为原先思路有误，故重新写。
//	timeval pav;
//
//	gettimeofday(&pav,NULL);
//
//	unsigned int millisecs = pav.tv_sec*1000 + pav.tv_usec/1000;
//
//	srand(millisecs);
//
//	int m = CCRANDOM_0_1() * (visibleSize.width + origin.x - 50) + 50;
//
//	int n = CCRANDOM_0_1() * 200 + 100;
//
//	m_pMonster->setPosition( m , visibleSize.height + origin.y);
//
//	switch (m_ePathType)
//	{
//	case MonsterPathType::LINE:
//		{
//			ActionInterval* pMove1 = MoveTo::create(2,Vec2(m_pMonster->getPositionX(),m_pMonster->getPositionY() - n ));
//
//			ActionInterval* pMove2 = MoveTo::create(0.7,Vec2(m_pMonster->getPositionX() - 50,m_pMonster->getPositionY() - n ));
//
//			CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterPlane::RepeatMove,this));
//
//			FiniteTimeAction* pAction = Sequence::create(pMove1,pMove2,pCallFunc,NULL);
//									
//			m_pMonster->runAction(pAction);
//		}
//		break;
//	case MonsterPathType::ARC:			// 这个表示boss
//		{
//			m_pMonster->setPosition(  visibleSize.width/2 + origin.x , visibleSize.height + origin.y);
//
//			ActionInterval* pMove1 = MoveTo::create(2,Vec2(m_pMonster->getPositionX(),m_pMonster->getPositionY() - 100 ));
//
//			ActionInterval* pMove2 = MoveTo::create(0.7,Vec2(m_pMonster->getPositionX() - 50,m_pMonster->getPositionY() - 100 ));
//
//			CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterPlane::RepeatMove,this));
//
//			FiniteTimeAction* pAction = Sequence::create(pMove1,pMove2,pCallFunc,NULL);
//
//			m_pMonster->runAction(pAction);
//
//		}
//		break;
//	case MonsterPathType::TRIANGLE:
//		{
//			ActionInterval* pMove1 = MoveTo::create(2,Vec2(m_pMonster->getPositionX(),m_pMonster->getPositionY() - n ));
//
//			ActionInterval* pMove2 = MoveTo::create(0.7,Vec2(m_pMonster->getPositionX() - 50,m_pMonster->getPositionY() - n  ));
//
//			CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterPlane::RepeatMove,this));
//
//			FiniteTimeAction* pAction = Sequence::create(pMove1,pMove2,pCallFunc,NULL);
//
//			m_pMonster->runAction(pAction);
//
//		}
//		break;
//	default:
//		break;
//	}
//}
//
//
//Rect MonsterPlane::GetMonsterRect()
//{
//	return m_pMonster->getBoundingBox();
//}
//
//void MonsterPlane::RepeatMove()
//{
//	ActionInterval* pMove = MoveBy::create(1.4,Vec2(100,0));
//
//	Action* pAction =  RepeatForever::create(Sequence::create(pMove,pMove->reverse(),NULL));
//
//	m_pMonster->runAction(pAction);
//}
//
//void MonsterPlane::DeleteMe()
//{
//	EnemyLayer::GetMe()->RemoveMonsterPlane(this);
//	this->getParent()->removeChild(this);
//}
//
//void MonsterPlane::OnpenFrie()
//{
//	MonsterBullet* pBullet = MonsterBullet::create();
//	if (pBullet)
//	{
//		if (m_ePathType == MonsterPathType::ARC)
//		{
//			pBullet->Init(GetMonsterTagId(),m_pMonster->getPositionX(),m_pMonster->getPositionY(),m_pMonster->getContentSize().height/2,true);
//
//		}else
//		{
//			pBullet->Init(GetMonsterTagId(),m_pMonster->getPositionX(),m_pMonster->getPositionY(),m_pMonster->getContentSize().height/2);
//		}
//
//		pBullet->SetAttack(GetAttack());
//		pBullet->InitAction();
//
//	}
//}
//
//void MonsterPlane::BeHit(int damage)
//{
//	if ( damage > 0 )
//	{
//		SetHp(GetHp() - damage);
//
//		if ( GetHp() <= 0 )
//		{
//			SetHp(0);
//
//			ActionInterval* blink = Blink::create(0.5,3);
//			//ActionInstant* pHide = Hide::create();
//			CallFunc* pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterPlane::DieCallback,this));
//			FiniteTimeAction* pAction = Sequence::create(blink,pCallFunc,NULL);
//			m_pMonster->runAction(pAction);
//
//		}else
//		{
//			if (!m_isPlayerAction)
//			{
//				m_isPlayerAction = true;
//
//				ActionInterval* blink = Blink::create(0.3,3);
//				CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterPlane::BeHitCallBacK,this));
//				m_pMonster->runAction(Sequence::create(blink,pCallFunc,NULL));
//			}
//		}
//	}
//}
//
//void MonsterPlane::BeHitCallBacK()
//{
//	m_isPlayerAction = false;
//	m_pMonster->setVisible(true);
//}
//
//void MonsterPlane::DieCallback()
//{
//	EnemyLayer::GetMe()->RemoveMonsterPlane(this);
//	BulletLayer::GetMe()->RemoveMonsterAllBulletWithPlaneId(GetMonsterTagId());
//	this->getParent()->removeChild(this);
//}
//
//void MonsterPlane::Tick(float dt)
//{
//	m_AllTime += dt;
//
//	if ( m_AllTime + 00.2 > m_nextFireTiem )
//	{
//		OnpenFrie();
//
//		m_nextFireTiem = m_AllTime + 0.5f;
//	}	
//}