#include "LXPlane.h"
#include "ObjectEx/DropItem/LXDropItem.h"
#include "ObjectEx/LXObjectManager.h"
#include "GameDefine.h"
#include "Scene/GameScene.h"

LXPlane::LXPlane()
	: LXObject()
{
}

LXPlane::~LXPlane() 
{
}

void LXPlane::Init(int value)
{
	LXObject::Init(value);
}

void LXPlane::Tick(float fDelta)
{
	LXObject::Tick(fDelta);

}

void LXPlane::Destroy()
{
	LXObject::Destroy();

}

void LXPlane::DropCrystal()
{
	bool bValue = true;
	
	if ( GetObjectType() == object_type_plane_player)
	{
		bValue = false;

	}

	std::vector<int> vec;
	vec.push_back(m_nDropItemNum);

	// 创建钻石
	CreatCrystal(bValue,vec);

}

void LXPlane::DropItem()
{
	// 掉落道具创建
	CreatItem( m_nDropItemID );
}

void LXPlane::CreatCrystal(bool DropGoodsCrystal,std::vector<int> vec)
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();
	float startX = 0.0f;
	float endX = screenSize.width;
	if (screenSize.width == 600) // 600*800加边框特殊处理
	{
		startX = 30;
		endX = 570;
	}

	int  tempVar = std::min( this->getContentSize().width,this->getContentSize().height ) / 4.0f ;		// 计算随机范围
	int	 tempNum = std::max( std::min( 3*m_nDropItemNum/10, 30 ), 10 );				// 计算物品数量

	for (int i = 0; i < vec.size(); i++)
	{
		// 确定掉落类型
		int nTempType = i + 6;
		int nType = nTempType;

		if ( DropGoodsCrystal )
		{
			nType += 3;
		}

		for (int j = 0; j < vec[i]; j++)
		{
			Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_ITEM);
			LXDropItem* pObject = dynamic_cast<LXDropItem*>(ObjectManager().CreateObject(nType, pParent, object_type_dropItem, false) );
			if (pObject)
			{
				pObject->Init( nType );
				pObject->SetTarget(true);
				pObject->SetTargetRange(-1);
				pObject->SetTargetMoveSpeed( 1200 );

				if ( tempNum - 10 + 1 > 0 )
				{
					pObject->SetTargetDelayTime( ( rand()%static_cast<int>( tempNum - 10 + 1 ) + 10 ) / 10.0F );
				}else
				{
					pObject->SetTargetDelayTime( 10 / 10.0F );
				}
				
				pObject->InitAction( 0.0f, 100, 100, 100, rand()%360, 0, 0, false, false, startX, 0, endX, screenSize.height );

				if ( tempVar == 0 )
				{
					tempVar += 1;
				}
				pObject->setPositionX( this->getPositionX() + rand()%static_cast<int>( 2*tempVar ) - tempVar );
				pObject->setPositionY( this->getPositionY() + rand()%static_cast<int>( 2*tempVar ) - tempVar );
			}
		}
	}
}

void LXPlane::CreatItem(int ItemId)
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();
	float startX = 0.0f;
	float endX = screenSize.width;
	if (screenSize.width == 600) // 600*800加边框特殊处理
	{
		startX = 30;
		endX = 570;
	}

	if ( ItemId != -1 )
	{
		Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_ITEM);
		LXDropItem* pObject = dynamic_cast<LXDropItem*>(ObjectManager().CreateObject(ItemId, pParent, object_type_dropItem, false) );
		if (pObject)
		{
			pObject->Init( ItemId );

			switch (ItemId)
			{
			case 1:
			case 2:
				{
					pObject->SetTarget(false);
					pObject->SetTargetRange(150);
					pObject->SetTargetMoveSpeed( 1000 );
					pObject->SetTargetDelayTime( 1.0f );
				}
				break;
			default:
				{
					pObject->SetTarget(true);
					pObject->SetTargetRange(-1);
					pObject->SetTargetMoveSpeed( 1000 );
					pObject->SetTargetDelayTime( 1.0f );
				}
				break;
			}

			pObject->InitAction( 0.0f, 100, 100, 100, rand()%360, 0, 0, false, false, startX, 0, endX, screenSize.height );
			pObject->setPosition( this->getPosition() );
		}
	}
}