#include "Animation.h"
#include "Frame.h"
#include "AnimationLogic.h"

LXAnimation::LXAnimation()
	: m_fOriginX(0.0f),
	m_fOriginY(0.0f),
	m_fWidth(0.0f),
	m_fHeight(0.0f),
	m_nFrameNum(0)
{
	for (int i = 0; i < 100; i++)
	{
		m_Frames[i] = nullptr;
	}
}

LXAnimation::LXAnimation(const LXAnimation& anim)
{
	m_nFrameNum		= 0;		
	for (int i = 0; i < anim.m_nFrameNum; i++)
	{
		LXFrame *pFrame = new LXFrame(*(anim.m_Frames[i]));
		AddFrame(pFrame);
	}

	m_Path			= anim.m_Path;		
	m_Name			= anim.m_Name;		
	m_Script		= anim.m_Script;		
	
	m_fOriginX		= anim.m_fOriginX;	
	m_fOriginY		= anim.m_fOriginY;	
	m_fWidth		= anim.m_fWidth;		
	m_fHeight		= anim.m_fHeight;	
	m_nFrameNum		= anim.m_nFrameNum;		
}

LXAnimation::~LXAnimation() 
{
	for (int i = 0; i < m_nFrameNum; i++)
	{
		delete m_Frames[i];
		m_Frames[i] = nullptr;
	}
}

void LXAnimation::AddFrame(LXFrame *pFrame)
{
	m_Frames[m_nFrameNum] = pFrame;
	m_nFrameNum++;
}

// 初始化关键帧信息
void LXAnimation::InitFrameInfo(LXAnimationLogic *pLogic, std::string &namefix)
{
	// 生成关键帧信息
	if (pLogic)
	{
		for(int i = 0; i < m_nFrameNum; i++)
		{
			LXFrame *pFrame = m_Frames[i];
			if (pFrame)
			{
				for(int j = 0; j < pFrame->GetDelay(); j++)
				{
					pLogic->AddFrame(i + 1);
				}
			}
		}
	}
}

// 初始化关键帧元素信息
void LXAnimation::InitElementsInfo(LXAnimationLogic *pLogic, Node *pNode, int frameIndex, std::string &nameFix, int transmode)
{
	// 生成渲染动画信息
	if (frameIndex <= m_nFrameNum && frameIndex >0)
	{
		LXFrame *pFrame = m_Frames[frameIndex - 1];
		if (pFrame)
		{
			pFrame->Render(this, pLogic, pNode, nameFix, transmode);
		}
	}
}
