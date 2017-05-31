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
	// 关键帧编辑
	void			AddElement(LXElement *element);
	
	// 渲染
	void			Render(LXAnimation *pAnimation, LXAnimationLogic* pLogic, Node *pNode, std::string &nameFix, int transmode);

public:
	void			SetDelay(int value) { m_nDelay = value; }
	int				GetDelay() { return m_nDelay; }
	
	void			SetScript(std::string &value) { m_Script = value; }
	std::string&	GetScript() { return m_Script; }

public:
	int			m_nDelay;		// 延迟
	std::string m_Script;		// 脚本

	int			m_nElementNum;	// 元素数量
	LXElement*  m_Elements[100];// 元素信息
};
