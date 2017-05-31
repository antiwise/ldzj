#pragma once
#include <vector>
#include "cocos2d.h"

//动画
USING_NS_CC;
class LXFrame;
class LXAnimationLogic;
class LXAnimation
{
public:
	LXAnimation();
	LXAnimation(const LXAnimation& anim);
	~LXAnimation();

public:
	// 关键帧编辑
	void			AddFrame(LXFrame *pFrame);
	
	// 初始化关键帧信息
	void			InitFrameInfo(LXAnimationLogic *pLogic, std::string &namefix);
	
	// 初始化关键帧元素信息
	void			InitElementsInfo(LXAnimationLogic *pLogic, Node *pNode, int frameIndex, std::string &nameFix, int transmode);

public:
	void			SetPath(std::string &value) { m_Path = value; }
	std::string&	GetPath() { return m_Path; }

	void			SetName(std::string &value) { m_Name = value; }
	std::string&	GetName() { return m_Name; }
	
	void			SetScript(std::string &value) { m_Script = value; }
	std::string&	GetScript() { return m_Script; }
	
	void			SetOriginX(float value) { m_fOriginX = value; }
	float			GetOriginX() { return m_fOriginX; }
	
	void			SetOriginY(float value) { m_fOriginY = value; }
	float			GetOriginY() { return m_fOriginY; }
	
	void			SetWidth(float value) { m_fWidth = value; }
	float			GetWidth() { return m_fWidth; }
	
	void			SetHeight(float value) { m_fHeight = value; }
	float			GetHeight() { return m_fHeight; }

public:
	std::string m_Path;			// 标志
	std::string m_Name;			// 动画名称
	std::string m_Script;		// 脚本

	float		m_fOriginX;		// 位置
	float		m_fOriginY;		// 位置
	float		m_fWidth;		// Size
	float		m_fHeight;		// Size

	int			m_nFrameNum;	// 动画帧数量
	LXFrame*	m_Frames[100];	// 动画帧
};
