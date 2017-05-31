#include "LXObject.h"
#include "Animation/AnimationManager.h"
#include "Animation/AnimationLogic.h"
#include "Action/ActionManager.h"
#include "Shooter/LXShooter.h"
#include "Shooter/LXShooterLogic.h"
#include "ObjectEx/LXObjectManager.h"
#include "GameDefine.h"
#include "Effect/LXEffectManager.h"
#include "Scene/GameScene.h"

LXObject::LXObject()
	: m_nResID(-1),
	m_pShooters(nullptr),
	m_pAnimationLogic(nullptr),
	m_pAction(nullptr),
	m_nCurrentHP(100),
	m_nTotalHP(100),
	m_bIsPlayer(false),
	m_nAttack(0),
	m_nAttackFactor(100),
	m_nDefense(0),
	m_fBulletDegree(0.0f),
	m_fBulletSpeed(600),
	m_fBulletDelta(1),
	m_fBulletDeltaSpeed(0.0f),
	m_fBulletMinSpeed(600),
	m_fBulletMaxSpeed(600),
	m_bBulletSpeedChange(false),
	m_fBulletMaxDeltaDegree(0.0f),
	m_bCleanFlag(false),
	m_bDeadFlag(false),
	m_nObjectType(object_type_none),
	m_nZOrder(GAME_ZORDER_ENEMY),
	m_bUIObject(false),
	m_pEffect(nullptr),
	m_fLastHitTime(0),
	m_bPause(false)
{
	// 创建渲染子节点
	m_pRenderNode = Node::create();
	addChild(m_pRenderNode, -1);
	m_pOBB = new ::OBB();
}

LXObject::~LXObject() 
{
	if(m_pAnimationLogic)
	{
		delete m_pAnimationLogic;
		m_pAnimationLogic = nullptr;
	}
}

void LXObject::Init(int value)
{
	m_nResID = value; 
}

void LXObject::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);

#if SHOWOBB
    m_CustomCommand.init(_globalZOrder);
    m_CustomCommand.func = CC_CALLBACK_0(LXObject::onDraw, this, transform);
    renderer->addCommand(&m_CustomCommand);
#endif

}

#if SHOWOBB
void LXObject::onDraw(const Mat4 &transform)
{
    Director* director = Director::getInstance();
    Mat4 oldModelView;
    oldModelView = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
	
	int x1 = m_Rect.origin.x;
	int y1 = m_Rect.origin.y;
	int x2 = m_Rect.origin.x + m_Rect.size.width;
	int y2 = m_Rect.origin.y + m_Rect.size.height;

	CCPoint pt1 = ccp(x1, y1);
	CCPoint pt2 = ccp(x2, y1);
	CCPoint pt3 = ccp(x2, y2);
	CCPoint pt4 = ccp(x1, y2);

	Vec2 vertices[4] = {
        pt1,
        pt2,
        pt3,
        pt4,
    };

    DrawPrimitives::drawPoly(vertices, 4, true);
	
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldModelView);
}
#endif

void LXObject::InitAction(std::string& eventID, int tag)
{
	LXActionManager().DoAction(this, eventID, tag);
}

void LXObject::PlayAnimaton(std::string& path, const std::string& name, std::string nameFix, int type, bool flip)
{
	m_Path = path;
	m_NameFix = nameFix;
	if(m_pAnimationLogic == nullptr)
	{
		m_pAnimationLogic = AnimationManager().CreateAnimationNode(m_pRenderNode, path, name, nameFix, flip);
		if (m_pAnimationLogic)
		{
			m_pAnimationLogic->SetObject(this);
			m_pAnchorPoint = m_pAnimationLogic->GetAnchorPoint();
			m_pAnimationSize = m_pAnimationLogic->GetAnimationSize();
			Point targetPos = ccpSub(m_pRenderNode->getPosition(), ccp(m_pAnchorPoint.x, -m_pAnchorPoint.y));
			m_pRenderNode->setPosition(targetPos);
			m_pAnimationLogic->SetType(type);
		}
	}
	else
	{
		m_pAnimationLogic->SetFlip(flip);
		m_pAnimationLogic->SetObject(this);
		m_pAnimationLogic->SetType(type);
		m_pAnimationLogic->PlayAnimation(path, name, nameFix);
		Point targetPos = ccpAdd(m_pRenderNode->getPosition(), ccp(m_pAnchorPoint.x, -m_pAnchorPoint.y));
		m_pAnchorPoint = m_pAnimationLogic->GetAnchorPoint();
		m_pAnimationSize = m_pAnimationLogic->GetAnimationSize();
		targetPos = ccpSub(targetPos, ccp(m_pAnchorPoint.x, -m_pAnchorPoint.y));
		m_pRenderNode->setPosition(targetPos);
	}
}

Rect LXObject::GetObjectBoundBox()
{
	Vec2 pos = getPosition();
	return Rect(m_Rect.getMinX() + pos.x, m_Rect.getMinY() + pos.y, m_Rect.getMaxX() + pos.x, m_Rect.getMaxY() + pos.y);
	//return Rect(m_Rect.getMinX() + pos.x, m_Rect.getMinY() + pos.y, m_Rect.getMinX() + pos.x + m_nSize, m_Rect.getMinY() + pos.y + m_nSize);
}

void LXObject::SetNextAnimation(const std::string& value)
{
	m_NextAnimation = value;
	m_pAnimationLogic->SetCallBack(true);
}

void LXObject::OnAnnimationEnd()
{
	if( m_NextAnimation.length() > 0)
	{
		std::string strPath = GetPath();
		PlayAnimaton(strPath, m_NextAnimation);
		m_NextAnimation = "";
		m_pAnimationLogic->SetCallBack(false);
	}
}

void LXObject::InitShooters(LXShooterGroup* pShooters)
{
	// 清理现有发射逻辑]
	if (m_pShooters == pShooters)
	{
		return;
	}
	m_pShooters = pShooters;

	for (int i = 0; i < m_Shooters.size(); i++)
	{
		m_Shooters[i]->StopAction();
		delete m_Shooters[i];
	}
	m_Shooters.clear();

	if (pShooters)
	{
		pShooters->InitShooterLogic(this, m_Shooters);
	}
}

void LXObject::InitShootLogic()
{
	for (int i = 0; i < m_Shooters.size(); i++)
	{
		LXShooterLogic *pLogic = m_Shooters[i];
		pLogic->InitAction();
	}
}

void LXObject::StopShootLogic()
{
	for (int i = 0; i < m_Shooters.size(); i++)
	{
		LXShooterLogic *pLogic = m_Shooters[i];
		pLogic->StopAction();
	}
}

void LXObject::MoveShooter(int index, float x, float y)
{
	if (index > 0 && index < m_Shooters.size())
	{
		m_Shooters[index]->ChangeAnchorPoint(x, y);
	}
}

void LXObject::InitShootLogicByIndex(std::vector<int> & vec)
{
	for (int i = 0; i < m_Shooters.size(); i++)
	{
		LXShooterLogic *pLogic = m_Shooters[i];
		pLogic->StopAction();
	}

	for (int i = 0; i < vec.size(); i++)
	{
		int key = vec[i];
		if (key < m_Shooters.size())
		{
			LXShooterLogic *pLogic = m_Shooters[key];
			pLogic->InitAction();
		}
	}
}

void LXObject::Tick(float fDelta)
{
	if (m_pAnimationLogic)
	{
		m_pAnimationLogic->Tick(fDelta);
	}

	if (m_pEffect)
	{
		if (m_nResID != 18 && m_nResID != 33 && m_nResID != 78)
		{
		}
		else
		{
			m_pEffect->setPosition(getPositionX(), getPositionY());
		}
	}
}

void LXObject::SetCollideInfo(Rect rect)
{
	m_Rect = rect;
	m_nSize = m_Rect.size.width > m_Rect.size.height?m_Rect.size.width:m_Rect.size.height;
}

::OBB* LXObject::GetCollideInfo()
{
	// 计算AABB
	Vec2 currentPos = getPosition();
	if (m_pLastPos == currentPos)
	{
		return m_pOBB;
	}

	m_pLastPos = currentPos;

	if (m_nObjectType >= object_type_weapon_bomb && m_fBulletDegree != 0.0f)
	{
		int x1 = m_Rect.origin.x;
		int y1 = m_Rect.origin.y;
		int x2 = m_Rect.origin.x + m_Rect.size.width;
		int y2 = m_Rect.origin.y + m_Rect.size.height;

		CCPoint pt = convertToWorldSpace(ccp(x1, y1));
		m_pOBB->setVertex(0, pt.x, pt.y);

		pt = convertToWorldSpace(ccp(x2, y1));
		m_pOBB->setVertex(1, pt.x, pt.y);

		pt = convertToWorldSpace(ccp(x2, y2));
		m_pOBB->setVertex(2, pt.x, pt.y);

		pt = convertToWorldSpace(ccp(x1, y2));
		m_pOBB->setVertex(3, pt.x, pt.y);
	}
	else
	{
		Vec2 pos = getPosition();
		float x = pos.x + m_Rect.origin.x;
		float y = pos.y + m_Rect.origin.y;
		m_pOBB->setVertex(0, x, y);
		m_pOBB->setVertex(1, x + m_Rect.size.width, y);
		m_pOBB->setVertex(2, x + m_Rect.size.width, y + m_Rect.size.height);
		m_pOBB->setVertex(3, x, y + m_Rect.size.height);
	}
	

	return m_pOBB;
}

void LXObject::PlayHitEffect()
{
	Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
	std::string strName = "p0/hited.p";
	EffectManager().CreateEffect(pParent,strName,this->getPositionX(), this->getPositionY(),GAME_ZORDER_OBJECT_PARTICLE,false);
}

void LXObject::Hit(int damage)
{
	m_nCurrentHP -= damage;

	// 战机损毁
	if (m_nCurrentHP <= 0)
	{
		m_bCleanFlag = true;
		m_bDeadFlag = true;
	}
}

void LXObject::Destroy()
{
	stopAllActions();	
	if (m_pEffect)
	{
		EffectManager().DeleteEffect(m_pEffect);
		m_pEffect = nullptr;
	}
}

// 清理
void LXObject::PlayDestroyEffect()
{
	
}

void LXObject::PlayEffect(std::string effectName)
{
	if (m_pEffect)
	{
		EffectManager().DeleteEffect(m_pEffect);
		m_pEffect = nullptr;
	}

	if (m_nResID != 18 && m_nResID != 33 && m_nResID != 78)
	{
		m_pEffect = EffectManager().CreateEffect(this,effectName, 0, 0,GAME_ZORDER_PLAYER_BULLET,true);
	}
	else
	{
		Node *pParent = nullptr;
		if(IsUIObject())
		{
			pParent = getParent();
		}
		else
		{
			pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_PLANE);
		}
		m_pEffect = EffectManager().CreateEffect(pParent,effectName, this->getPositionX(), this->getPositionY(),GAME_ZORDER_PLAYER_BULLET,true);
	}
}