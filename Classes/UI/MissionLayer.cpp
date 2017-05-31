#include "MissionLayer.h"
#include "Scene/MainScene.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "Animation/AnimationLogic.h"
#include "Animation/AnimationManager.h"
#include "DataBase/DataBase.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"
#include "MyAnimation/TuHaoJin.h"
#include "MyAnimation/NiceGirl.h"

std::string defaultAnimationName = "g_0";
std::string defaultFixedName = "";

MissionLayer::MissionLayer():Layer()
{
	m_SelectIndex = 0;					// 选择的关卡ID;
	m_NewAttackMission = 0;				// 要攻打的新关卡

	m_ReturnBtn = nullptr;				// 返回按钮
	m_GoToBtn = nullptr;				// 出击

	m_ToLeftImage = nullptr;			// 向左
	m_ToRightImage = nullptr;			// 向右
	m_SaoMiaoBack = nullptr;			// 扫描背景
	m_SaoMiaoImage = nullptr;			// 扫描图片

	m_BossInfoBack = nullptr;			// boss显示信息

	m_PageView = nullptr;				// 星球滑动页
	
	m_BossInfoNode = nullptr;			// boss信息父节点

	m_TiShiInfo = nullptr;
	m_TishiTime = 0.0f;

	m_isRefreshStarName = false;

	m_MissionCount = 6;

	//SoundSystem().preloadEffect(Mic_button);
	//SoundSystem().preloadEffect(Mic_back);

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Texture/Res/ui/guanqiaxuanze.plist","Texture/Res/ui/guanqiaxuanze.png");
	LXFileUtils::AddSpriteFrameCache("ui/select_rank.plist","ui/select_rank.png");
}

MissionLayer::~MissionLayer()
{
	m_AnimParent.clear();
	m_pAnimationLogic.clear();
}

bool MissionLayer::init()
{
	//// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	return true;
}

bool MissionLayer::Init()
{
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(MISSION_COUNT_INDEX);

	if (pParamInfo)
	{
		m_MissionCount = pParamInfo->getIntValue();
	}

	for (int i = 0; i < m_MissionCount; i++)
	{
		m_AnimParent.push_back(nullptr);

		m_pAnimationLogic.push_back(nullptr);
	}

	LoadGuanQiaXuanZeJson();

	InitStarInfo();


	/*---------------------------------------------------------------------*/
	// 礼包添加
	TuHaoJin* _tuhaojin = TuHaoJin::create();
	if (_tuhaojin)
	{
		_tuhaojin->Init(0.75f,0.f);
		_tuhaojin->setPosition( Utils().GetFixedPosion( 232, 60 ) );
		this->addChild( _tuhaojin );
	}
	return true;
}

void MissionLayer::Tick(float dt)
{
	for (int i = 0; i < m_pAnimationLogic.size(); i++)
	{
		if ( m_pAnimationLogic[i] )
		{
			if ( i == m_SelectIndex )
			{
				m_pAnimationLogic[m_SelectIndex]->Tick(dt);
			}
		}
	}


	RefreshStarState();

	if ( m_TiShiInfo && m_TiShiInfo->isVisible() )
	{
		m_TishiTime += dt;

		if ( m_TishiTime >= 1.2f )
		{
			m_TiShiInfo->setVisible(false);
			m_TishiTime = 0.0f;
		}
	}
}

void MissionLayer::LoadGuanQiaXuanZeJson()
{
	// 加载json文件
	std::string key = "guanqiaxuanze";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pGuanQianXuanZeLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!pGuanQianXuanZeLayout)
	{
		return ;
	}

	this->addChild(pGuanQianXuanZeLayout);

	// 返回
	m_ReturnBtn = static_cast<Button*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "returnBtn"));
	m_ReturnBtn->addTouchEventListener(CC_CALLBACK_2(MissionLayer::ReturnTouchEvent, this));

	// 出击
	m_GoToBtn = static_cast<Button*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "GoOnBtn"));
	m_GoToBtn->addTouchEventListener(CC_CALLBACK_2(MissionLayer::GoOnTouchEvent, this));

	m_ToLeftImage = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "ImageZuo"));
	m_ToLeftImage->addTouchEventListener(CC_CALLBACK_2(MissionLayer::ToLeftTouchEvent, this));

	m_ToRightImage = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "ImageYou"));
	m_ToRightImage->addTouchEventListener(CC_CALLBACK_2(MissionLayer::ToRightTouchEvent, this));

	// 扫描
	Layout*		SaoMiaoBack = static_cast<Layout*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "saomiaobeijing"));
	ImageView*	SaoMiaoImage = static_cast<ImageView*>(Helper::seekWidgetByName(SaoMiaoBack, "saomiao"));
	SaoMiaoBack->setVisible(true);
	SaoMiaoImage->setVisible(true);

	Vec2 SaoMiaoBack_Pos  = SaoMiaoBack->getPosition();
	Size SaoMiaoBack_Size = SaoMiaoBack->getContentSize();

	Vec2 SaoMiaoImage_Pos  = SaoMiaoImage->getPosition();
	Size SaoMiaoImage_Size = SaoMiaoImage->getContentSize();

	// 扫描动画
	ActionInterval* pMoveTo = MoveTo::create(0,Vec2(SaoMiaoImage_Pos.x,SaoMiaoBack_Size.height*2));
	ActionInterval* pMoveby = MoveBy::create(3,Vec2(0,-(SaoMiaoBack_Size.height *2 + SaoMiaoImage_Size.height/4 ) ));
	ActionInterval* pSequence = Sequence::create(pMoveTo,pMoveby,NULL);
	FiniteTimeAction* pAction = RepeatForever::create(pSequence);
	SaoMiaoImage->runAction(pAction);

	// boss信息
	m_BossInfoBack = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "bossInfo"));

	// 翻页控件
	m_PageView = static_cast<PageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "PageView"));
	m_PageView->addEventListener(CC_CALLBACK_2(MissionLayer::PageViewEvent, this));

	// 页数标识
	ImageView*	pageImage_1 = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "page1"));
	ImageView*	pageImage_2 = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "page2"));
	m_vecPageNum.push_back(pageImage_1);
	m_vecPageNum.push_back(pageImage_2);

	m_TiShiInfo = static_cast<ImageView*>(Helper::seekWidgetByName(pGuanQianXuanZeLayout, "tishi"));
	m_TiShiInfo->setVisible(false);

}

void MissionLayer::InitStarInfo()
{
	if (!m_PageView)
	{
		return ;
	}

	/*---------------------------------------------------------------------*/
	// 参考 b.c.bd 中 bd构造函数
	DBC_DEFINEHANDLE(_MissionData, DBC_MISSION_INFO);
	const int length = _MissionData->GetRecordsNum();

	if ( m_MissionCount > length )
	{
		MessageBox(("关卡配置错误，MissionInfo.txt表中数据数量小于Parameter.txt 35的值.请检查."),"Error!!!");
		return ;
	}

	Layout* layout = nullptr;

	for (int i = 0; i < m_MissionCount; i++)
	{
		_DBC_MISSION_INFO *pMissionInfo = (_DBC_MISSION_INFO*)_MissionData->Search_LineNum_EQU( i );

		if ( !pMissionInfo )
		{
			log(" read mission file error !");
			return ;
		}

		if ( i % 3 == 0 )
		{
			// 创建控件
			layout = Layout::create();

			if (layout)
			{
				// 加入页面 
				m_PageView->addPage( layout );
			}

		}

		if (!layout)
		{
			return ;
		}

		// 设置大小
		layout->setContentSize(Size(480, 430));

		// 后面两个转圈的
		Node* _node = Node::create();
		if ( !_node )
		{
			return ;
		}

		// 添加进节点中
		layout->addChild( _node );
		m_vecRepeatNode.push_back(_node);

		// 外圈
		ImageView* circleImage_1 = ImageView::create( "guanqiaxuanze_10.png",Widget::TextureResType::PLIST);

		// 内圈
		ImageView* circleImage_2 = ImageView::create( "guanqiaxuanze_09.png",Widget::TextureResType::PLIST);

		if ( !( circleImage_1 && circleImage_2 ) )
		{
			return ;
		}

		circleImage_1->setPosition( Vec2( pMissionInfo->GetStarPosX(), pMissionInfo->GetStarPosY() ) );
		circleImage_2->setPosition( Vec2( pMissionInfo->GetStarPosX(), pMissionInfo->GetStarPosY() ) );

		_node->addChild( circleImage_1 );
		_node->addChild( circleImage_2 );

		// 创建2个动画
		ActionInterval* _Rotate1 = RotateBy::create(9.2f,360);
		Action* _Repeat1 = RepeatForever::create( _Rotate1 );
		circleImage_1 -> runAction( _Repeat1 );

		ActionInterval* _Rotate2 = RotateBy::create(9.3f,-360);		
		Action* _Repeat2 = RepeatForever::create( _Rotate2 );
		circleImage_2 -> runAction( _Repeat2 );

		// 星球
		std::string strPath = pMissionInfo->GetStarImage();

		ImageView* starImage = ImageView::create( strPath,Widget::TextureResType::PLIST);
		if ( !starImage )
		{
			return ;
		}

		layout->addChild( starImage );

		starImage->setPosition( Vec2( pMissionInfo->GetStarPosX(), pMissionInfo->GetStarPosY() ) );
		starImage->setTouchEnabled( true );
		starImage->addTouchEventListener( CC_CALLBACK_2( MissionLayer::StarTouchEvent, this, i ) );

		// 星球名
		ImageView* starName = ImageView::create("select_rank_003.png",Widget::TextureResType::PLIST);
		if (!starName)
		{
			return ;
		}

		layout->addChild(starName);

		starName->setPositionX( pMissionInfo->GetStarPosX() );
		starName->setPositionY( pMissionInfo->GetStarPosY() - starImage->getContentSize().height/2 - starName->getContentSize().height );
		m_vecStarName.push_back(starName);
	}

}

void MissionLayer::ShowStarSelectAnim()
{
	int length = m_vecRepeatNode.size();
	for ( int i = 0; i < length; i++ )
	{
		if ( i == m_SelectIndex )
		{
			m_vecRepeatNode[i]->setVisible( true );
		}else
		{
			m_vecRepeatNode[i]->setVisible( false );
		}
	}
}

void MissionLayer::ShowPageIndex()
{
	int idx = m_PageView->getCurPageIndex();
	for ( int i = 0; i < m_vecPageNum.size(); i++ )
	{
		if ( i == idx )
		{
			m_vecPageNum[i]->setVisible( true );
		}else
		{
			m_vecPageNum[i]->setVisible( false );
		}
	}
}

void MissionLayer::InitData(bool bIsBoss /* = false */)
{

	if (m_PageView )
	{
		int nIdx = 0;
		int nAimMission = 1;

		if ( bIsBoss == false )
		{
			nAimMission = DBManager().GetData(DATA_NEXT_MISSION) ;
			nIdx = ( nAimMission - 1 ) / 3;
		}else
		{
			nAimMission = DBManager().GetData(DATA_NEXT_BOSS);
			nIdx = ( nAimMission - 1 ) / 3;
		}

		if ( nIdx > m_PageView->getPages().size() )
		{
			this->setVisible(false);
			return ;
		}

		m_PageView->scrollToPage( nIdx );

		SetMissonInfo( nAimMission - 1 );

	}
}


void MissionLayer::RefreshStarState()
{
	if ( DBManager().IsBossMission() == false )
	{
		if( m_NewAttackMission != DBManager().GetData(DATA_NEXT_MISSION) )
		{
			m_NewAttackMission = DBManager().GetData(DATA_NEXT_MISSION);

			m_isRefreshStarName = true;
		}

	}else
	{
		if( m_NewAttackMission != DBManager().GetData(DATA_NEXT_BOSS) )
		{
			m_NewAttackMission = DBManager().GetData(DATA_NEXT_BOSS);

			m_isRefreshStarName = true;
		}
	}

	if ( !m_isRefreshStarName )
	{
		return ;
	}

	for ( int i = 0; i < m_vecStarName.size(); i++)
	{
		DBC_DEFINEHANDLE(_MissionData, DBC_MISSION_INFO);
		_DBC_MISSION_INFO *pMissionInfo = (_DBC_MISSION_INFO*)_MissionData->Search_LineNum_EQU( i );

		if (pMissionInfo)
		{
			if ( m_NewAttackMission >= ( i + 1 ) )
			{
				std::string strPath = pMissionInfo->GetStarNameImage();
				m_vecStarName[i]->loadTexture( strPath,Widget::TextureResType::PLIST);
			}else
			{
				std::string strPath = pMissionInfo->GetLockStarNameImage();
				m_vecStarName[i]->loadTexture( strPath,Widget::TextureResType::PLIST);
			}
		}	
	}

	m_isRefreshStarName = false;
}


void MissionLayer::ReturnTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			static_cast<MainScene*>(this->getParent())->HideMissonLayer();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void MissionLayer::GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			log("CHUJISTAT==");
			switch ( DBManager().GetCurrPlaneID() )
			{
			case 1:
				{
					// 没有激活2号飞机
					log("CHUJISTAT==222");
					if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
					{
						dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

						return ;
					}
					log("CHUJISTAT==222   END");
				}
				break;
			case 2:
				{
					log("CHUJISTAT==333   ");
					// 没有激活3号飞机
					if ( DBManager().GetData(DATA_HAVE_PLANE3) <= 0 )
					{
						dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane3);

						return ;

					}
					log("CHUJISTAT==333   END");
				}
				break;
			case 4:
				{
					log("CHUJISTAT==  444   ");
					// 没有激活4号飞机
					if ( DBManager().GetData(DATA_HAVE_PLANE4) <= 0 )
					{
						dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane4);

						return ;
					}
					log("CHUJISTAT==444   END");
				}
				break;
			default:
				break;
			}
			log("IsBossMission  FALSE");
			if ( DBManager().IsBossMission() == false )
			{
				if ( m_SelectIndex + 1 <= DBManager().GetData(DATA_NEXT_MISSION) )
				{
					log("GAMESCENE START");
					static_cast<MainScene*>(this->getParent())->SetMisson( m_SelectIndex + 1 );
					static_cast<MainScene*>(this->getParent())->TurntoGameSence();
					log("XINSHOU JIAOXUE ");
					// 新手教学
					if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
					{
						
						if ( NiceGirl::GetFreshManCurrStep() == 2 || NiceGirl::GetFreshManCurrStep() == 3 )
						{
							NiceGirl::SetFreshManCurrStep( 4 );
							log("XINSHOU JIAOXUE ");
							dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();

						}
					}

				}
				else
				{
					m_TiShiInfo->setVisible(true);
					m_TishiTime = 0.0f;
				}

			}else
			{
				if ( m_SelectIndex + 1 <= DBManager().GetData(DATA_NEXT_BOSS) )
				{
					static_cast<MainScene*>(this->getParent())->SetMisson( m_SelectIndex + 1 );
					static_cast<MainScene*>(this->getParent())->TurntoGameSence();
				}
				else
				{
					m_TiShiInfo->setVisible(true);
					m_TishiTime = 0.0f;
				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void MissionLayer::StarTouchEvent(Ref* pSender, Widget::TouchEventType type, int idx)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SetMissonInfo(idx);
			SoundSystem().playEffect(Mic_button);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void MissionLayer::SetMissonInfo(int mission)
{
	//设置关卡state
	m_SelectIndex = mission;

	SetBossInfo();
	ShowStarSelectAnim();
	ShowPageIndex();
}

// 翻页回调
void MissionLayer::PageViewEvent(Ref* pSender,	PageView::EventType type)
{
	switch (type)
	{
	case PageView::EventType::TURNING:
		{
			ShowPageIndex();
		}
		break;
	default:
		break;
	}
}


void MissionLayer::SetBossInfo()
{
	DBC_DEFINEHANDLE(_MissionData, DBC_MISSION_INFO);
	_DBC_MISSION_INFO *pMissionInfo = (_DBC_MISSION_INFO*)_MissionData->Search_Index_EQU( m_SelectIndex + 1 );

	if ( !m_pAnimationLogic[m_SelectIndex] )
	{
		std::string strPath = pMissionInfo->GetBossFileName();

		m_AnimParent[m_SelectIndex] = Node::create();
		if(!m_AnimParent[m_SelectIndex])
		{
			return ;
		}

		m_pAnimationLogic[m_SelectIndex] = AnimationManager().CreateAnimationNode(m_AnimParent[m_SelectIndex], strPath, g_AnimationName_g0, defaultFixedName, false);
		if (!m_pAnimationLogic[m_SelectIndex])
		{
			return ;
		}

		m_pAnimationLogic[m_SelectIndex]->SetType(animation_type_repeat);

		m_AnimParent[m_SelectIndex]->setPositionX( -20 + std::atof( pMissionInfo->GetBossOffsetX() ) );
		m_AnimParent[m_SelectIndex]->setPositionY( 230 + std::atof( pMissionInfo->GetBossOffsetY() ) );
		m_AnimParent[m_SelectIndex]->setScale( std::atof( pMissionInfo->GetBossScale() ) );

		m_BossInfoBack->addChild( m_AnimParent[m_SelectIndex] );

	}

	// 停止其它的动画
	for (int i = 0; i < m_AnimParent.size(); i++)
	{
		if ( m_AnimParent[i] )
		{
			if ( i == m_SelectIndex )
			{
				m_AnimParent[i] ->setVisible(true);
			}else
			{
				m_AnimParent[i] ->setVisible(false);
			}
		}
	}

	/*---------------------------------------------------------------------*/
	// 显示设置
	int nTempLength = DBManager().GetData(DATA_NEXT_MISSION);
	if ( DBManager().IsBossMission() )
	{
		nTempLength = DBManager().GetData(DATA_NEXT_BOSS);
	}

	if ( m_SelectIndex >= nTempLength  )
	{
		if ( !m_pAnimationLogic[m_SelectIndex] )
		{
			return ;
		}
		// 进行偏色
		m_pAnimationLogic[m_SelectIndex]->SetColor( Color3B( 0.1f*255, 0.1f*255, 0.1f*255 ) );

		if (! m_BossInfoNode )							// 未通关
		{
			m_BossInfoNode = Node::create();
			if (!m_BossInfoNode)
			{
				return ;
			}

			m_BossInfoNode->setContentSize(m_BossInfoBack->getContentSize());
			m_BossInfoNode->setAnchorPoint(Vec2( 0.0f,0.0f ));

			m_BossInfoBack->addChild(m_BossInfoNode);

		}else
		{
			m_BossInfoNode->removeAllChildren();
		}


		ImageView* image1 = ImageView::create("select_rank_047.png",Widget::TextureResType::PLIST);
		if (!image1)
		{
			return ;
		}

		image1->setPosition(Vec2(310,190));
		m_BossInfoNode->addChild(image1);

		ImageView* image2 = ImageView::create("select_rank_050.png",Widget::TextureResType::PLIST);
		if (!image2)
		{
			return ;
		}

		image2->setPosition(Vec2(320,155));
		m_BossInfoNode->addChild(image2);

		ImageView* image3 = ImageView::create("select_rank_049-1.png",Widget::TextureResType::PLIST);
		if (!image3)
		{
			return ;
		}

		image3->setPosition(Vec2(310,75));
		m_BossInfoNode->addChild(image3);


	}else			// 未通关
	{
		if ( ! m_pAnimationLogic[m_SelectIndex] )
		{
			return ;
		}
		// 取消偏色
		m_pAnimationLogic[m_SelectIndex]->SetColor( Color3B::WHITE );

		if (! m_BossInfoNode )
		{
			m_BossInfoNode = Node::create();
			if (!m_BossInfoNode)
			{
				return ;
			}

			m_BossInfoNode->setContentSize(m_BossInfoBack->getContentSize());
			m_BossInfoNode->setAnchorPoint(Vec2( 0.0f,0.0f ));

			m_BossInfoBack->addChild(m_BossInfoNode);

		}else
		{
			m_BossInfoNode->removeAllChildren();
		}


		std::string strBossName = pMissionInfo->GetBossNameImage();
		std::string strBossInfo = pMissionInfo->GetBossInfoImage();

		// 名字
		ImageView* image1 = ImageView::create( strBossName, Widget::TextureResType::PLIST );
		if (!image1)
		{
			return ;
		}

		image1->setPosition(Vec2(317,190));
		m_BossInfoNode->addChild(image1);

		// 介绍
		ImageView* image2 = ImageView::create( strBossInfo, Widget::TextureResType::PLIST );
		if (!image2)
		{
			return ;
		}

		image2->setPosition(Vec2(320,135));
		m_BossInfoNode->addChild(image2);

		// 血、攻、速 背景
		ImageView* image3 = ImageView::create("select_rank_049.png",Widget::TextureResType::PLIST);
		if (!image3)
		{
			return ;
		}

		image3->setPosition(Vec2(305,60));
		m_BossInfoNode->addChild(image3);


		// 血
		const int AttackNum = pMissionInfo->GetBossHp();
		for (int i = 0; i < AttackNum; i++)
		{
			ImageView* imageDian = ImageView::create("select_rank_048.png",Widget::TextureResType::PLIST);
			if (!imageDian)
			{
				return ;
			}

			imageDian->setPosition(Vec2(46 + i*13,55));
			image3->addChild(imageDian);
		}

		// 攻
		const int hpNum = pMissionInfo->GetBossAttack();
		for (int i = 0; i < hpNum; i++)
		{
			ImageView* imageDian = ImageView::create("select_rank_048.png",Widget::TextureResType::PLIST);
			if (!imageDian)
			{
				return ;
			}

			imageDian->setPosition(Vec2(46 + i*13,33));
			image3->addChild(imageDian);
		}

		// 速
		const int speedNum = pMissionInfo->GetBossSpeed();
		for (int i = 0; i < speedNum; i++)
		{
			ImageView* imageDian = ImageView::create("select_rank_048.png",Widget::TextureResType::PLIST);
			if (!imageDian)
			{
				return ;
			}

			imageDian->setPosition(Vec2(46 + i*13,10));
			image3->addChild(imageDian);
		}

	}
}


void MissionLayer::ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			int totalPages = m_PageView->getPages().size();
			int currentPage = m_PageView->getCurPageIndex();

			if (currentPage > 0)
			{
				m_PageView->scrollToPage(currentPage - 1);

				ShowPageIndex();
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void MissionLayer::ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			int totalPages = m_PageView->getPages().size();
			int currentPage = m_PageView->getCurPageIndex();

			if (currentPage < totalPages - 1)
			{
				m_PageView->scrollToPage(currentPage + 1);

				ShowPageIndex();
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}