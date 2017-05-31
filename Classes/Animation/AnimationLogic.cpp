#include "AnimationLogic.h"
#include "Animation.h"
#include "FrameLogic.h"
#include "ElementLogic.h"
#include "AnimationManager.h"
#include "ObjectEx/LXObject.h"
#include "Common/FileUtils.h"

LXAnimationLogic::LXAnimationLogic(Node *pParent, LXAnimation* pAnimation, std::string &nameFix, bool flip)
	: m_pParent(pParent),
	m_pAnimation(pAnimation),
	m_pObject(NULL),
	m_NameFix(nameFix),
	m_nSpriteIndex(0),
	m_nType(animation_type_normal),
	m_nCurrentFrame(1),
	m_nFramePos(0),
	m_fTime(0),
	m_fDelta(1.f / 30),
	m_bFlip(flip),
	m_Color(Color3B::WHITE),
	m_nAnimationNum(0),
	m_bCallBack(false),
	m_nFrameNum(0)
{
	for (int i = 0; i < 100; i++)
	{
		m_SpritePool[i] = nullptr;
		m_FrameInfo[i] = 0;
	}

	// 初始化动画信息
	initAnimationInfo();
}

LXAnimationLogic::~LXAnimationLogic() 
{
}

Point LXAnimationLogic::GetAnchorPoint() 
{ 
	return Point(m_pAnimation->GetOriginX(), m_pAnimation->GetOriginY());
}

Size LXAnimationLogic::GetAnimationSize() 
{ 
	return Size(m_pAnimation->GetWidth(), m_pAnimation->GetHeight());
}

bool LXAnimationLogic::PlayAnimation(std::string &name, const std::string &key, std::string &nameFix)
{
	// 判断动画文件是否已经解析
	AnimationManager().LoadAnimation(name);
	// 切换播放的动画
	if (key == m_pAnimation->GetName() && name == m_pAnimation->GetPath())
	{
		return true;
	}

	std::string animKey = AnimationManager().GetKeyName(name, key);
	m_pAnimation = AnimationManager().GetAnimation(animKey);
	if (m_pAnimation)
	{
		m_NameFix = nameFix;
		initAnimationInfo();
		
		return true;
	}

	return false;
}

// 增加关键帧
void LXAnimationLogic::AddFrame(int value)
{
	m_FrameInfo[m_nFrameNum] = value;
	m_nFrameNum++;
}

void LXAnimationLogic::initAnimationInfo()
{
	if (m_pParent == NULL || m_pAnimation == NULL)
	{
		return;
	}
	
	resetFrameInfo();

	// 解析帧信息
	m_pAnimation->InitFrameInfo(this, m_NameFix);
	m_pAnimation->InitElementsInfo(this, m_pParent, 1, m_NameFix, m_bFlip);

	// 数据成员初始化
	if (m_nType == animation_type_reverse || m_nType == animation_type_rrepeat)
	{
		m_nCurrentFrame		= m_nFrameNum - 1;
		m_nFramePos			= m_nFrameNum - 1;
	}
	else
	{
		m_nCurrentFrame		= 0;
		m_nFramePos			= 0;
	}

	m_fDelta			= 1.0f / 30;
	m_fTime				= 0;
	m_nSpriteIndex		= 0;
}

void LXAnimationLogic::resetFrameInfo()
{
	for (int i = 0; i < 100; i++)
	{
		m_FrameInfo[i] = 0;
	}
	
	m_nFrameNum = 0;
	
	// 隐藏全部子元素
	for (int i = 0; i < m_nAnimationNum; i++)
	{
		if (m_SpritePool[i] != nullptr)
		{
			m_SpritePool[i]->bState = false;
			m_SpritePool[i]->pSprite->setVisible(false);
			m_SpritePool[i]->pSprite->setOpacity(255);
		}
	}
}

int LXAnimationLogic::calcFrame(float fDelta)
{
	int KeyFrame = 1;
	int frame = (int)(m_fTime / m_fDelta);
	m_fTime += fDelta;
	
	if (m_nFrameNum > 1 && frame >=0)
	{
		switch (m_nType)
		{
		case animation_type_normal:
			{
				frame = std::min(m_nFrameNum - 1, frame);
			}
			break;
		case animation_type_reverse:
			{
				frame = std::max(m_nFrameNum - frame - 1, 0);
			}
			break;
		case animation_type_repeat:
			{
				frame = frame % m_nFrameNum;
			}
			break;
		case animation_type_rrepeat:
			{
				frame = frame % m_nFrameNum;
				frame = m_nFrameNum - frame - 1;
			}
			break;
		case animation_type_pingpong:
			{
				frame = frame % (m_nFrameNum * 2 - 2);
				if (frame > m_nFrameNum)
				{
					frame = (m_nFrameNum - 2 - (frame - m_nFrameNum));
				}
			}
			break;
		case animation_type_random:
			{
				frame = rand() % (m_nFrameNum - 1);
			}
			break;
		default:
			{
				frame = std::min(m_nFrameNum - 1, frame);
			}
			break;
		}

		KeyFrame = m_FrameInfo[frame];
		m_nFramePos = frame;
	}
	else
	{
		m_nFramePos = 0;
		KeyFrame = 1;
	}

	return KeyFrame;
}

bool LXAnimationLogic::isAnimationEnd()
{
	switch (m_nType)
	{
	case animation_type_normal:
		{
			if (m_nFramePos != m_nFrameNum - 1)
			{
				return false;
			}
		}
		break;
	case animation_type_reverse:
		{
			if (m_nFramePos != 0)
			{
				return false;
			}
		}
		break;
	case animation_type_repeat:
		{
			if (m_nFramePos != m_nFrameNum - 1)
			{
				return false;
			}
		}
		break;
	case animation_type_rrepeat:
		{
			if (m_nFramePos != 0)
			{
				return false;
			}
		}
		break;
	case animation_type_pingpong:
		{
			if (m_nFramePos != 1)
			{
				return false;
			}
		}
		break;
	case animation_type_random:
		{
			return false;
		}
		break;
	default:
		{
			if (m_nFramePos != m_nFrameNum - 1)
			{
				return false;
			}
		}
		break;
	}

	return true;
}

// Tick逻辑
void LXAnimationLogic::Tick(float fDelta)
{
	if (m_nFrameNum == 1)
	{
		return;
	}

	if (m_bCallBack && isAnimationEnd() && m_pObject)
	{
		m_pObject->OnAnnimationEnd();
	}

	// 计算逻辑帧信息
	int keyFrame = calcFrame(fDelta);

	// 判断当前逻辑帧是否一致
	if (keyFrame != m_nCurrentFrame)
	{ 
		// 隐藏全部子元素
		for (int i = 0; i < m_nAnimationNum; i++)
		{
			if (m_SpritePool[i] != nullptr)
			{
				m_SpritePool[i]->bState = false;
				m_SpritePool[i]->pSprite->setVisible(false);
			}
		}

		m_nCurrentFrame = keyFrame;
		// 渲染
		m_pAnimation->InitElementsInfo(this, m_pParent, m_nCurrentFrame, m_NameFix, m_bFlip);
	}
}

Sprite* LXAnimationLogic::GetRenderElement(std::string &path, std::string &key)
{
	for (int i = 0; i < m_nAnimationNum; i++)
	{
		if (m_SpritePool[i] && m_SpritePool[i]->bState == false && m_SpritePool[i]->key == key)
		{
			m_SpritePool[i]->bState = true;
			return m_SpritePool[i]->pSprite;
		}
	}
	
	AnimRenderElement* pRender = new AnimRenderElement();
	pRender->key = key;
	pRender->bState = true;
	pRender->pSprite = Sprite::create();
	if (key == "none")
	{
	}
	else
	{
		std::string plist = path + ".plist";
		std::string png = path + ".png";
		LXFileUtils::AddSpriteFrameCache(plist, png); 
		pRender->pSprite->setSpriteFrame(key + ".png");
	}
	m_SpritePool[m_nAnimationNum] = pRender;
	m_nAnimationNum++;

	return pRender->pSprite;
}

// 设置颜色
void LXAnimationLogic::SetColor(Color3B color) 
{ 
	m_Color = color; 
	m_pAnimation->InitElementsInfo(this, m_pParent, 1, m_NameFix, m_bFlip);
}