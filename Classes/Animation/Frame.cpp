#include "Frame.h"
#include "Element.h"
#include "AnimationLogic.h"

LXFrame::LXFrame()
	: m_nDelay(1),
	m_nElementNum(0)
{
	for (int i = 0; i < 100; i++)
	{
		m_Elements[i] = nullptr;
	}
}

LXFrame::LXFrame(const LXFrame &frame)
{
	m_nElementNum	= 0;
	for (int i = 0; i < frame.m_nElementNum; i++)
	{
		LXElement *pElement = new LXElement(*frame.m_Elements[i]);
		AddElement(pElement);
	}

	m_nDelay		= frame.m_nDelay;		
	m_Script		= frame.m_Script;	
	m_nElementNum	= frame.m_nElementNum;	
}

LXFrame::~LXFrame()
{
	for (int i = 0; i < m_nElementNum; i++)
	{
		delete m_Elements[i];
		m_Elements[i] = nullptr;
	}
}

void LXFrame::AddElement(LXElement *element)
{
	m_Elements[m_nElementNum] = element;
	m_nElementNum++;
}

void LXFrame::Render(LXAnimation *pAnimation, LXAnimationLogic* pLogic, Node *pNode, std::string &nameFix, int transmode)
{
	if (pLogic && pNode)
	{
		// äÖÈ¾
		for (int i = 0; i < m_nElementNum; i++)
		{
			LXElement *pElement = m_Elements[i];
			if (pElement)
			{
				// äÖÈ¾×ÓÔªËØ
				pElement->Render(pAnimation, pLogic, pNode, nameFix, transmode);
			}
		}
	}
}
