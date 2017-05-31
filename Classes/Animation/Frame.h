#pragma once
#include <vector>
#include "cocos2d.h"

USING_NS_CC;
class LXElement;
class LXAnimation;
class LXAnimationLogic;
class LXFrame
{
public:
	LXFrame();
	LXFrame(const LXFrame &frame);
	~LXFrame();
	
public:
	// �ؼ�֡�༭
	void			AddElement(LXElement *element);
	
	// ��Ⱦ
	void			Render(LXAnimation *pAnimation, LXAnimationLogic* pLogic, Node *pNode, std::string &nameFix, int transmode);

public:
	void			SetDelay(int value) { m_nDelay = value; }
	int				GetDelay() { return m_nDelay; }
	
	void			SetScript(std::string &value) { m_Script = value; }
	std::string&	GetScript() { return m_Script; }

public:
	int			m_nDelay;		// �ӳ�
	std::string m_Script;		// �ű�

	int			m_nElementNum;	// Ԫ������
	LXElement*  m_Elements[100];// Ԫ����Ϣ
};
