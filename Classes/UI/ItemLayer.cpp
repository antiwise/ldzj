//#include "ItemLayer.h"
//#include "ObjectEx/LXObjectManager.h"
//
//ItemLayer* ItemLayer::s_ItemLayer = nullptr;
//
//ItemLayer::ItemLayer()
//{
//	s_ItemLayer = this;
//
//	m_AllItemNum = 0;
//}
//
//ItemLayer::~ItemLayer()
//{
//
//}
//
//bool ItemLayer::init()
//{
//	if ( !Layer::init() )
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void ItemLayer::MonsterDropOut(Vec2 MonsterPos,int eTypeIdx /* = 0 */)
//{
//
//	timeval pav;
//
//	gettimeofday(&pav,NULL);
//
//	unsigned int millisecs = pav.tv_sec*1000 + pav.tv_usec/1000;
//
//	srand(millisecs);
//
//	int m = CCRANDOM_0_1()*(2*40) - 20;
//
//	int n = CCRANDOM_0_1()*(2*40) - 20;
//
//	int r = CCRANDOM_0_1()*359;
//
//
//	// µôÂäÎïÆ·
//	LXDropItem* pObject = new LXDropItem();
//	pObject->Init(eTypeIdx);
//	pObject->setPositionX(MonsterPos.x + m);
//	pObject->setPositionY(MonsterPos.y + n);
//
//	pObject->SetTarget(true);
//	pObject->SetTargetRange(150);
//	pObject->SetTargetMoveSpeed(1000);
//	pObject->SetTargetDelayTime(1.5);
//	pObject->InitAction(0.5,
//		100,
//		100,
//		100,
//		r,
//		0,
//		0,
//		false,
//		false,
//		0, 0, 480, 800);
//
//	this->addChild(pObject, 1000, GetLXObjectManager()->GetTag());
//	GetLXObjectManager()->AddDropItem(pObject);
//
//}
//
//void ItemLayer::Tick(float dt)
//{
//	/*++ m_AllItemNum;
//	if ( m_AllItemNum % 10 == 1)
//	{
//		MonsterDropOut(Vec2(240,500),6);
//	}*/
//}