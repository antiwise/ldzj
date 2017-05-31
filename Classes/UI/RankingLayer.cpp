#include "RankingLayer.h"
#include "DataBase/DataBase.h"
#include "GameDefine.h"
#include "Common/FileUtils.h"
#include "Scene/MainScene.h"
#include "TaskOption/TaskOptionMgr.h"
#include "DB/DBManager.h"
#include "Sound/LXSoundSystem.h"
#include "SetName.h"

#define RewardBtnTag	888

bool TaskSort(TaskOption* _a,TaskOption* _b)
{
	if ( _b->m_eTaskState > _a->m_eTaskState )
	{
		return true;
	}

	return false;
}

RankingLayer::RankingLayer(void):Layer()
{
	m_DeleteID = -1;
	m_isNeedDelete = false;

	m_CurrRanking = 5001;
	m_CurrScore = -1;
}


RankingLayer::~RankingLayer(void)
{
	for ( auto it : m_vecRanking )
	{
		delete it;
	}

	m_vecRanking.clear();

}

bool RankingLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	LXFileUtils::AddSpriteFrameCache("ui/ranking.plist","ui/ranking.png");

	std::string key = "ranking";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pRankingLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!pRankingLayout)
	{
		return false;
	}

	this->addChild(pRankingLayout);

	// 继续按钮
	Button* _closeButton = static_cast<Button*>(Helper::seekWidgetByName(pRankingLayout, "guanbiButton"));
	_closeButton->addTouchEventListener(CC_CALLBACK_2(RankingLayer::CloseCallback, this));

	// 成就按钮
	m_RewardButton = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "chengjiu"));
	m_RewardButton->addTouchEventListener(CC_CALLBACK_2(RankingLayer::ClickRewardCallback, this));

	// 排行按钮
	m_RankingButton = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "paiming"));
	m_RankingButton->addTouchEventListener(CC_CALLBACK_2(RankingLayer::ClickRankingCallback, this));

	// 列表容器
	m_ListView = static_cast<ListView*>(Helper::seekWidgetByName(pRankingLayout, "ListView"));
	m_ListView->addEventListener((ListView::ccListViewCallback)(CC_CALLBACK_2(RankingLayer::SelectedItemEvent, this)));

	// 排名背景
	m_RankingBackGround = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "beijing"));

	// 第一图标
	m_FirstIcon = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "jin"));

	// 第二图标
	m_SecondIcon = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "yin"));

	// 第三图标
	m_ThreeIcon = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, "tong"));

	// 加载背景图片
	const int length = 9;
	for (int i = 0; i < length; i++)
	{
		char tempName[32] = {0};
		sprintf(tempName,"Image_%d",i);
		ImageView* pImage = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, tempName));
		m_vecBackImage.push_back(pImage);
	}

	// 加载icon图标
	for (int i = 0; i < length; i++)
	{
		char tempName[32] = {0};
		sprintf(tempName,"icon_%d",i);
		ImageView* pImage = static_cast<ImageView*>(Helper::seekWidgetByName(pRankingLayout, tempName));
		m_vecIcon.push_back(pImage);
	}

	// 加载名次
	for (int i = 0; i < length; i++)
	{
		char tempName[32] = {0};
		sprintf(tempName,"mingci_%d",i);
		Text* pText = static_cast<Text*>(Helper::seekWidgetByName(pRankingLayout, tempName));
		m_vecPlace.push_back(pText);
	}

	// 加载姓名
	for (int i = 0; i < length; i++)
	{
		char tempName[32] = {0};
		sprintf(tempName,"name_%d",i);
		Text* pText = static_cast<Text*>(Helper::seekWidgetByName(pRankingLayout, tempName));
		m_vecName.push_back(pText);
	}

	// 加载分数
	for (int i = 0; i < length; i++)
	{
		char tempName[32] = {0};
		sprintf(tempName,"num_%d",i);
		Text* pNum = static_cast<Text*>(Helper::seekWidgetByName(pRankingLayout, tempName));
		m_vecScore.push_back(pNum);
	}

	/*---------------------------------------------------------------------*/
	// 为了效率 成就奖励界面只加载一次
	//LoadRewardInfo();

	return true;
}

void RankingLayer::Init()
{
	// 显示
	this->setVisible(true);

	if (m_isCalcRanking)
	{
		LoadRankingInfo();
	}

	// 默认显示第一页
	ShowRewardInfo();
}

void RankingLayer::Tick(float dt)
{
	if (m_isNeedDelete)
	{
		auto it = m_listRewardInfo.begin();
		while ( it != m_listRewardInfo.end() )
		{
			if( (*it)->getTag() == m_DeleteID )
			{
				ssize_t _widget = m_ListView->getIndex( *it );
				m_ListView->removeItem(_widget);

				it = m_listRewardInfo.erase( it );


				TaskMgr().Reward( m_DeleteID );

				m_isNeedDelete = false;
				return ;
			}else
			{
				it ++ ;
			}
		}
	}
}

void RankingLayer::LoadRewardInfo()
{
	if (!m_ListView)
	{
		return ;
	}

	m_DeleteID = -1;
	m_isNeedDelete = false;

	// 获取成就奖励数据
	DBC_DEFINEHANDLE(s_pAchieveDBC, DBC_ACHIEVE);
	if (!s_pAchieveDBC)
	{
		return ;
	}

	for( auto it : TaskMgr().GetTaskOptionMap() )
	{
		if ( it.second->m_eTaskState == TASK_STATE::TASK_STATE_CLOSE )
		{
			continue;
		}

		const _DBC_ACHIEVE *m_pAchieveData = (const _DBC_ACHIEVE*)s_pAchieveDBC->Search_Index_EQU( it.second->m_nTaskID );
		if (!m_pAchieveData)
		{
			log("Error: file[%s][%s][%d]",__FILE__,__FUNCTION__,__LINE__);
		}

		// ----------- 创建 --------------------------------------
		// 背景条
		ImageView*	pImageBack = ImageView::create("ranking_02.png",Widget::TextureResType::PLIST);
		pImageBack->setScale(0.92);


		DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
		const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(16);
		const _DBC_PARAM_INFO *pParamInfo1 = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(17);
		const _DBC_PARAM_INFO *pParamInfo2 = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(18);
		const _DBC_PARAM_INFO *pParamInfo3 = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(21);

		const _DBC_PARAM_INFO *pYaHei = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(22);
		if (!( pParamInfo && pParamInfo1 && pParamInfo2 && pParamInfo3 && pYaHei  ) )
		{
			return ;
		}

		std::stringstream IconImagePath ;
		std::stringstream RewardInfo;
		switch ( it.second->m_eRewardType )
		{
		case REWARD_TYPE::REWARD_DIAMOND:
			{
				IconImagePath << "ranking_03.png";
				RewardInfo << pParamInfo->getStringValue() << "x" <<m_pAchieveData->nNum;
			}
			break;
		case REWARD_TYPE::REWARD_SHIELD:
			{
				IconImagePath << "ranking_20.png";
				RewardInfo <<  pParamInfo1->getStringValue() << "x" <<m_pAchieveData->nNum;
			}
			break;
		case REWARD_TYPE::REWARD_SKILL:
			{
				IconImagePath << "ranking_19.png";
				RewardInfo <<  pParamInfo2->getStringValue() << "x" <<m_pAchieveData->nNum;
			}
			break;
		default:
			break;
		}
		// icon
		ImageView*	pIcon = ImageView::create(IconImagePath.str(),Widget::TextureResType::PLIST);
		pIcon->setScale(0.92);

		// 任务名
		Text* pName = Text::create( m_pAchieveData->szName, pYaHei->getStringValue(),16);
		pName->setColor(Color3B(254,222,8));

		// 说明1
		Text* pInfo = Text::create( pParamInfo3->getStringValue(), pYaHei->getStringValue(),14);
		pInfo->setColor(Color3B(29,199,255));

		// 说明2
		Text* pInfo2 = Text::create( m_pAchieveData->szDescribe, pYaHei->getStringValue(),14);
		pInfo2->setAnchorPoint(Vec2(0,0));
		pInfo2->setColor(Color3B(236,244,255));

		// 奖励类容
		Text* Reward = Text::create( RewardInfo.str().c_str(), pYaHei->getStringValue(),14);
		Reward->setAnchorPoint(Vec2(0,0));
		Reward->setColor(Color3B(236,244,255));

		// 领奖
		Button* ReceiveBtn = Button::create("ranking_10.png","ranking_11.png","",Widget::TextureResType::PLIST);
		ReceiveBtn->setScale(0.92);
		ReceiveBtn->setTag(RewardBtnTag);
		ReceiveBtn->addTouchEventListener(CC_CALLBACK_2(RankingLayer::ReceiveCallback, this,m_pAchieveData->nID));

		Layout* Custom_item = Layout::create();
		//Custom_item->ignoreContentAdaptWithSize(false);		//  是否高亮
		Custom_item->setAnchorPoint(Vec2(0,0));
		Custom_item->setTouchEnabled(false);
		Custom_item->setContentSize( pImageBack->getContentSize() );
		Custom_item->retain();
		Custom_item->setTag( m_pAchieveData->nID );

		//--- 添加 -----------------------------------
		pImageBack->setPosition(Vec2(pImageBack->getContentSize().width/2,pImageBack->getContentSize().height/2));
		Custom_item->addChild(pImageBack);

		pIcon->setPosition(Vec2(95,43));
		Custom_item->addChild(pIcon);

		pName->setPosition(Vec2(174,61));
		Custom_item->addChild(pName);

		pInfo->setPosition(Vec2(150,37));
		Custom_item->addChild(pInfo);

		pInfo2->setPosition(Vec2(170,32));
		Custom_item->addChild(pInfo2);

		Reward->setPosition(Vec2(170,16));
		Custom_item->addChild(Reward);

		ReceiveBtn->setPosition(Vec2(390,40));
		Custom_item->addChild(ReceiveBtn);
		ReceiveBtn->setVisible(false);

		m_ListView->pushBackCustomItem(Custom_item);

		// 记录
		m_listRewardInfo.push_back( Custom_item );

	}

	m_ListView->setItemsMargin(-5.0f);
}

void RankingLayer::ShowRewardInfo()
{
	// ------------------界面处理------------------------------------------------
	// 设置图片
	m_RewardButton->loadTexture("ranking_07.png",Widget::TextureResType::PLIST);
	m_RankingButton->loadTexture("ranking_04.png",Widget::TextureResType::PLIST);

	// 隐藏排名相关窗口
	m_RankingBackGround->setVisible(false);
	m_FirstIcon->setVisible(false);
	m_SecondIcon->setVisible(false);
	m_ThreeIcon->setVisible(false);

	for ( auto it : m_vecBackImage )
	{
		it->setVisible(false);
	}

	for ( auto it : m_vecIcon )
	{
		it->setVisible(false);
	}

	for ( auto it : m_vecPlace )
	{
		it->setVisible(false);
	}

	for ( auto it : m_vecName )
	{
		it->setVisible(false);
	}

	for ( auto it : m_vecScore )
	{
		it->setVisible(false);
	}

	// 显示成就奖励相关
	m_ListView -> setVisible(true);

	m_ListView->jumpToTop();

	// --------------逻辑处理------------------------------------------------
	//std::sort( m_listRewardInfo.begin(),m_listRewardInfo.end(),TaskSort );
	//m_listRewardInfo.sort();

	for ( auto it : m_listRewardInfo )
	{
		TaskOption* tempTask =  TaskMgr().GetTaskOption( it->getTag() ) ;

		if ( tempTask && tempTask -> m_eTaskState == TASK_STATE_REWARD )
		{
			it->getChildByTag(RewardBtnTag) ->setVisible( true );
		}else
		{
			it->getChildByTag(RewardBtnTag) ->setVisible( false );
		}
	}

}

void RankingLayer::LoadRankingInfo()
{
	for ( auto it : m_vecRanking )
	{
		delete it;
	}

	m_vecRanking.clear();

	if ( DBManager().GetPlayerName().empty() || DBManager().GetPlayerName() == "Player" )
	{
		// 起名
		SetName* _SetName = SetName::create();
		_SetName->Init(OpenSetNameType_Ranking);
		dynamic_cast<MainScene*>(this->getParent())->addChild( _SetName, 111 );

		return ;
	}
	
	m_CurrRanking = CalcMyRankingWithMyScore(m_CurrScore);

	DBManager().SetData( DATA_WORLD_RANK,m_CurrRanking );

	const int length = 9;
	for (int i = 0; i < length; i++)
	{
		std::string OtherName = RandomOtherName();

		int OtherRanking = CalcRanking(i);

		int OtherScore = CalcScore(OtherRanking);

		RankingInfo* info = new RankingInfo();

		if ( OtherRanking == m_CurrRanking )
		{
			info->_Name		=  DBManager().GetPlayerName();
			info->_Randking	= m_CurrRanking;
			info->_Score	= m_CurrScore;

		}else
		{
			info->_Name		= OtherName;
			info->_Randking	= OtherRanking;
			info->_Score	= OtherScore;
		}

		m_vecRanking.push_back(info);
	}

	m_isCalcRanking = false;
}

void RankingLayer::ShowRankingInfo()
{
	// -- 界面处理 -----------------------------------------------------
	// 设置图片
	m_RewardButton->loadTexture("ranking_06.png",Widget::TextureResType::PLIST);
	m_RankingButton->loadTexture("ranking_05.png",Widget::TextureResType::PLIST);

	// 隐藏成就奖励相关
	m_ListView -> setVisible(false);

	// 显示排名相关窗口
	m_RankingBackGround->setVisible(true);

	for ( auto it : m_vecBackImage )
	{
		it->setVisible(true);
	}

	for ( auto it : m_vecIcon )
	{
		it->setVisible(true);
	}

	for ( auto it : m_vecPlace )
	{
		it->setVisible(true);
	}
	for ( auto it : m_vecName )
	{
		it->setVisible(true);
	}

	for ( auto it : m_vecScore )
	{
		it->setVisible(true);
	}

	// -- 逻辑处理 --------------------------------------------
	if (m_vecRanking.empty())
		return ;

	const int length = 9;

	if ( m_CurrRanking < 9 )
	{
		// 显示金，银，铜 图标
		m_FirstIcon->setVisible(false);
		m_SecondIcon->setVisible(false);
		m_ThreeIcon->setVisible(false);

		for (int i = 0; i < length; i++)
		{
			if ( m_vecRanking[i]->_Randking == m_CurrRanking )
			{
				m_vecBackImage[i]->loadTexture("ranking_12.png",Widget::TextureResType::PLIST);
			}else
			{
				m_vecBackImage[i]->loadTexture("ranking_13.png",Widget::TextureResType::PLIST);
			}

			if ( m_vecRanking[i] )
			{
				m_vecPlace[i]->setString( StringUtils::toString( m_vecRanking[i]->_Randking ) );
				m_vecName[i]->setString(  G2U( m_vecRanking[i]->_Name.c_str() ) );
				m_vecScore[i]->setString( StringUtils::toString( m_vecRanking[i]->_Score ) );
			}


			if ( i == 0 )
			{
				if ( m_vecRanking[i]->_Randking == 1 )
				{
					m_FirstIcon->loadTexture("ranking_15.png",Widget::TextureResType::PLIST);
					m_FirstIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);

				}else if( m_vecRanking[i]->_Randking == 2 )
				{
					m_FirstIcon->loadTexture("ranking_16.png",Widget::TextureResType::PLIST);
					m_FirstIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);

				}else if( m_vecRanking[i]->_Randking == 3 )
				{
					m_FirstIcon->loadTexture("ranking_17.png",Widget::TextureResType::PLIST);
					m_FirstIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);
				}else
				{
					m_FirstIcon->setVisible(false);

					m_vecPlace[i]->setVisible(true);
					m_vecIcon[i]->setVisible(true);
				}

			}else if ( i == 1 )
			{
				if( m_vecRanking[i]->_Randking == 2 )
				{
					m_SecondIcon->loadTexture("ranking_16.png",Widget::TextureResType::PLIST);
					m_SecondIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);

				}else if( m_vecRanking[i]->_Randking == 3 )
				{
					m_SecondIcon->loadTexture("ranking_17.png",Widget::TextureResType::PLIST);
					m_SecondIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);
				}else
				{
					m_SecondIcon->setVisible(false);

					m_vecPlace[i]->setVisible(true);
					m_vecIcon[i]->setVisible(true);
				}

			}else if( i == 2 )
			{
				if( m_vecRanking[i]->_Randking == 3 )
				{
					m_ThreeIcon->setVisible(true);

					m_vecPlace[i]->setVisible(false);
					m_vecIcon[i]->setVisible(false);
				}else
				{
					m_ThreeIcon->setVisible(false);

					m_vecPlace[i]->setVisible(true);
					m_vecIcon[i]->setVisible(true);
				}
			}else
			{
				m_vecPlace[i]->setVisible(true);
				m_vecIcon[i]->setVisible(true);
			}
		}
	}else
	{
		m_FirstIcon->setVisible(false);
		m_SecondIcon->setVisible(false);
		m_ThreeIcon->setVisible(false);

		for (int i = 0; i < length; i++)
		{
			if ( m_vecRanking[i] )
			{
				if ( m_vecRanking[i]->_Randking == m_CurrRanking )
				{
					m_vecBackImage[i]->loadTexture("ranking_12.png",Widget::TextureResType::PLIST);
				}else
				{
					m_vecBackImage[i]->loadTexture("ranking_13.png",Widget::TextureResType::PLIST);
				}

				m_vecPlace[i]->setString( StringUtils::toString( m_vecRanking[i]->_Randking ) );
				m_vecName[i]->setString(  G2U( m_vecRanking[i]->_Name.c_str() ) );
				m_vecScore[i]->setString( StringUtils::toString( m_vecRanking[i]->_Score ) );
			}
		}
	}
}

void RankingLayer::CloseCallback(Ref* pSender, Widget::TouchEventType type)
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
			this->setVisible(false);
			dynamic_cast<MainScene*>(this->getParent())->CloseRankingLayer();
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


void RankingLayer::ClickRewardCallback(Ref* pSender, Widget::TouchEventType type)
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
			ShowRewardInfo();
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


void RankingLayer::ClickRankingCallback(Ref* pSender, Widget::TouchEventType type)
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
			ShowRankingInfo();
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


void RankingLayer::ReceiveCallback(Ref* pSender, Widget::TouchEventType type,int id)
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
			m_isNeedDelete = true;
			m_DeleteID = id;

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

void RankingLayer::SelectedItemEvent(Ref* pSender, ListView::EventType type)
{
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{

		}
		break;
	case ListView::EventType::ON_SELECTED_ITEM_END:
		{

		}
		break;
	default:
		break;
	}
}


void RankingLayer::SetMyScore(int nNum)
{
	if ( m_CurrScore != nNum )
	{
		m_CurrScore = nNum;
		m_isCalcRanking = true;
	}else
	{
		m_isCalcRanking = false;
	}
}

int RankingLayer::CalcMyRankingWithMyScore(int nMyScore)
{
	for(int i = 0; i < 10; ++i)
	{
		if( nMyScore > 2 * g_ScoreIdx[i]) 
		{
			return i + 1;
		}
	}

	if(nMyScore > 800)
	{
		float var = 0.0F - (float)nMyScore;
		int tempVar = (int)( ( 47.764652252197266 - std::sqrt( 2281.462F - 0.23105861F * var ) ) / 0.11552930623292923 );
		return var > 0 ? 5000 - tempVar : 5000 - (int)( ( 47.764652252197266 + std::sqrt( 2281.462F - var * 0.23105861F) ) / 0.11552930623292923 );
	} else 
	{
		return 5001;
	}
}

int RankingLayer::CalcRanking(int idx)
{
	// 每页显示9个
	int nOtherRanking = 0;

	if ( m_CurrRanking > 5000 )
	{
		nOtherRanking = -9 + idx + 5001;
	}else
	{
		if ( m_CurrRanking > ( 5001 - 9 - 1) )
		{
			nOtherRanking = idx + m_CurrRanking;

		}else
		{
			if ( m_CurrRanking <= 4 )
			{
				nOtherRanking = idx + 1;
			}else
			{
				nOtherRanking = idx - 4 + m_CurrRanking;
			}
		}
	}

	return nOtherRanking;
}

int RankingLayer::CalcScore(int randking)
{
	int nScore = 0;
	if ( randking < 10 )
	{
		nScore = 2 * g_ScoreIdx[ randking - 1];
	}else
	{
		int nTemp = 5000 -  randking;
		int nValue = (int)(0.0f + 0.057764653f * (float)(nTemp * nTemp) + -47.764652f * (float)nTemp);
		nScore = 100*( nValue/100 );// 为了将后两位变成00

		if ( nScore <= 0 )
		{
			return 200;
		}
	}

	return nScore;
}

std::string RankingLayer::RandomOtherName()
{

	std::string name;

	int m = rand() % 50;

	if (m <= 0)
	{
		int n = rand() % 200;
		name = g_FirstName[ n ];
	}else if ( m < 2 )
	{
		int n = rand() % 199;
		name = g_SecondNmae[ n ];
	}else
	{
		std::string str;

		const std::string tempStr = "abcdefghijklmnopqrstuvwxyz";

		int length = rand() % 8;

		for (int i = 0; i < length; i++)
		{
			int _offset  = rand() % tempStr.length();
			str += tempStr.at(_offset);
		}


		length = rand() % 10;
		if ( length == 1 )
		{
			str += "_";
		}

		length = rand() % 3;
		for (int i = 0; i < length; i++)
		{
			if ( rand() % 2 )
			{
				str += StringUtils::toString( rand() % 10000 );
			}
		}

		if( str.length() < 5 ) 
		{
			if ( rand() % 2 )
			{
				int _offset  = rand() % 26;
				str += tempStr.at(_offset);
			}

			str += StringUtils::toString( rand() % 10000 );
		}

		if( str.length() < 5 ) 
		{
			str += StringUtils::toString( rand() % 100000 );
		}

		if( str.length() > 11 )
		{
			str = str.substr(0, 12);
		}

		name = str;
	}

	return name;

}

std::string RankingLayer::RandomMyName()
{
	int m = rand() % 3;

	std::string name;

	if ( m <= 0 )
	{
		int n = rand() % 200;
		name = g_SecondNmae[ n ];
	}else
	{
		int n = rand() % 200;
		name = g_FirstName[ n ];
	}

	return name;
}