#include "Element.h"
#include "AnimationLogic.h"
#include "AnimationManager.h"
#include "Animation.h"

std::string defaultNodePathName = "node";
std::string defaultNodeKeyName = "none";
LXElement::LXElement()
	: m_ResType(0), 
	m_nCurFrame(0),
	m_fX(0.0f),
	m_fY(0.0f),
	m_fWidth(0.0f),
	m_fHeight(0.0f),
	m_fOriginX(0.0f),
	m_fOriginY(0.0f),
	m_fScaleX(0.0f),
	m_fScaleY(0.0f),
	m_fRotation(0.0f),
	m_nTransMode(0),
	m_nArgb(0),
	m_pAnimation(nullptr)
{
}

LXElement::LXElement(const LXElement &element)
{
	m_ResDir		= element.m_ResDir;	
	m_ResName		= element.m_ResName;		
	m_ResType		= element.m_ResType;		
	m_Name			= element.m_Name;	
	m_nCurFrame		= element.m_nCurFrame;	
	m_fX			= element.m_fX;		
	m_fY			= element.m_fY;			
	m_fWidth		= element.m_fWidth;		
	m_fHeight		= element.m_fHeight;	
	m_fOriginX		= element.m_fOriginX;	
	m_fOriginY		= element.m_fOriginY;	
	m_fScaleX		= element.m_fScaleX;		
	m_fScaleY		= element.m_fScaleY;		
	m_fRotation		= element.m_fRotation;	
	m_nTransMode	= element.m_nTransMode;	
	m_nArgb			= element.m_nArgb;		
	
	m_Script		= element.m_Script;
}

LXElement::~LXElement()
{
}

void LXElement::Render(LXAnimation *pAnimation, LXAnimationLogic *pLogic, Node *pNode, std::string &nameFix, int transmode)
{
	if (pNode)
	{
		// 渲染子元素
		if (m_ResType == resource_type_pic)
		{
			if (m_ResName.length() == 0)
			{
				return;
			}

			Sprite *pSprite = nullptr;
			if(nameFix.size() > 0)
			{
				std::string targetName = m_TargetName + nameFix;
				pSprite = pLogic->GetRenderElement(m_ResDir, targetName);
			}
			else
			{
				pSprite = pLogic->GetRenderElement(m_ResDir, m_TargetName);
			}

			if (pSprite)
			{
				if(pSprite->getParent() == nullptr)
				{
					pNode->addChild(pSprite);
				}
				else
				{
					if (pSprite->getParent() != pNode)
					{
						pSprite->retain();
						pSprite->removeFromParent();
						pNode->addChild(pSprite);
						pSprite->release();
					}
				}

				pSprite->setVisible(true);
				pSprite->setAnchorPoint(ccp(m_fOriginX / m_fWidth, (m_fHeight - m_fOriginY) / m_fHeight));
				Vec2 targetPos;
				// 设置位置
				switch(transmode)
				{
				case 1:
					{
						targetPos = ccpAdd(ccp(pAnimation->GetWidth() - m_fX, -m_fY), ccp(-m_fOriginX, -m_fOriginY));
					}
					break;
				case 2:
					{
						targetPos = ccpAdd(ccp(m_fX, -(pAnimation->GetHeight() - m_fY)), ccp(m_fOriginX, -m_fOriginY));
					}
					break;
				case 3:
					{
						targetPos = ccpAdd(ccp(pAnimation->GetWidth() - m_fX, -(pAnimation->GetHeight() - m_fY)), ccp(m_fOriginX, -m_fOriginY));
					}
					break;
				default:
					{
						targetPos = ccpAdd(ccp(m_fX, -m_fY), ccp(m_fOriginX, -m_fOriginY));
					}
					break;
				}

				pSprite->setPosition(targetPos);
				pSprite->setScaleX(m_fScaleX);
				pSprite->setScaleY(m_fScaleY);
				pSprite->setColor(pLogic->GetColor());
				if (m_nArgb != -1)
				{
					float alpha = (float)((-16777216 & m_nArgb) >> 24);
					pSprite->setOpacity(alpha);
				}
				
				int targetMode = m_nTransMode ^ transmode;
				int targetDegree = m_fRotation;
				if (m_fRotation != 0.0f && m_fRotation != 360.0f && targetMode != 0)
				{
					switch(targetMode)
					{
					case 1:
						{
							targetDegree = -m_fRotation;
							pSprite->setFlipX(true);
							pSprite->setFlipY(false);
						}
						break;
					case 2:
						{
							targetDegree = 180 - m_fRotation;
							pSprite->setFlipX(false);
							pSprite->setFlipY(true);
						}
						break;
					case 3:
						{
							targetDegree = 180 + m_fRotation;
							pSprite->setFlipX(true);
							pSprite->setFlipY(true);
						}
						break;
					default:
						{
						}
						break;
					}
				}
				else
				{
					switch(targetMode)
					{
					case 0:
						{
							pSprite->setFlipX(false);
							pSprite->setFlipY(false);
						}
						break;
					case 1:
						{
							pSprite->setFlipX(true);
							pSprite->setFlipY(false);
						}
						break;
					case 2:
						{
							pSprite->setFlipX(false);
							pSprite->setFlipY(true);
						}
						break;
					case 3:
						{
							pSprite->setFlipX(true);
							pSprite->setFlipY(true);
						}
						break;
					default:
						{
							pSprite->setFlipX(false);
							pSprite->setFlipY(false);
						}
						break;
					}
				}
				pSprite->setRotation(targetDegree);
			}
		}
		else
		{
			// 获取子元素
			if (m_pAnimation == nullptr)
			{
				m_pAnimation = AnimationManager().GetAnimation(m_TargetAnimName);
			}
			
			if(m_pAnimation)
			{
				Sprite *pSprite = pLogic->GetRenderElement(defaultNodePathName, defaultNodeKeyName);
				if (pSprite)
				{
					if(pSprite->getParent() == nullptr)
					{
						pNode->addChild(pSprite);
					}
					else
					{
						if (pSprite->getParent() != pNode)
						{
							pSprite->retain();
							pSprite->removeFromParent();
							pNode->addChild(pSprite);
							pSprite->release();
						}
					}

					pSprite->setVisible(true);
					pSprite->setContentSize(Size(m_fWidth, m_fHeight));
					pSprite->setAnchorPoint(ccp(m_fOriginX / m_fWidth, (m_fHeight - m_fOriginY) / m_fHeight));
					pSprite->setScaleX(m_fScaleX);
					pSprite->setScaleY(m_fScaleY);
					pSprite->setRotation(360 - m_fRotation);
					pSprite->setPosition(ccpAdd(ccp(m_fX, -m_fY), ccp(m_fOriginX, m_fOriginY - (1 - m_fScaleY) * m_fHeight)));
				}

				int targetMode = m_nTransMode ^ transmode;
				m_pAnimation->InitFrameInfo(nullptr, nameFix);
				m_pAnimation->InitElementsInfo(pLogic, pSprite, m_nCurFrame + 1, nameFix, targetMode);
			}
		}
	}
}

void LXElement::SetResDir(std::string &value) 
{ 
	m_ResDir = value; 
	m_TargetName = m_ResDir + "_" + m_ResName; 
	m_TargetAnimName = AnimationManager().GetKeyName(m_ResDir, m_ResName); 
}
void LXElement::SetResName(std::string &value)
{ 
	m_ResName = value; 
	m_TargetName = m_ResDir + "_" + m_ResName; 
	m_TargetAnimName = AnimationManager().GetKeyName(m_ResDir, m_ResName); 
}