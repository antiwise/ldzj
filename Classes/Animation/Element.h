#pragma once
#include <string>
#include "cocos2d.h"

enum
{
	resource_type_pic = 0,		// 图片
	resource_type_child			// 子动画
};

USING_NS_CC;
class LXAnimationLogic;
class LXAnimation;
class LXElement
{
public:
	LXElement();
	LXElement(const LXElement &element);
	~LXElement();

public:
	void			Render(LXAnimation *pAnimation, LXAnimationLogic *pLogic, Node *pNode, std::string &nameFix, int transmode);

public:

	void			SetResDir(std::string &value);
	std::string&	GetResDir() { return m_ResDir; }

	void			SetResName(std::string &value);
	std::string&	GetResName() { return m_ResName; }

	void			SetResType(int value) { m_ResType = value; }
	int				GetResType() { return m_ResType; }

	void			SetName(std::string &value) { m_Name = value; }
	std::string&	GetName() { return m_Name; }
	
	void			SetCurFrame(int value) { m_nCurFrame = value; }
	int				GetCurFrame() { return m_nCurFrame; }
	
	void			SetX(float value) { m_fX = value; }
	float			GetX() { return m_fX; }
	
	void			SetY(float value) { m_fY = value; }
	float			GetY() { return m_fY; }
	
	void			SetWidth(float value) { m_fWidth = value; }
	float			GetWidth() { return m_fWidth; }
	
	void			SetHeight(float value) { m_fHeight = value; }
	float			GetHeight() { return m_fHeight; }
	
	void			SetOriginX(float value) { m_fOriginX = value; }
	float			GetOriginX() { return m_fOriginX; }
	
	void			SetOriginY(float value) { m_fOriginY = value; }
	float			GetOriginY() { return m_fOriginY; }
	
	void			SetScaleX(float value) { m_fScaleX = value; }
	float			GetScaleX() { return m_fScaleX; }
	
	void			SetScaleY(float value) { m_fScaleY = value; }
	float			GetScaleY() { return m_fScaleY; }
	
	void			SetRotation(float value)
	{ 
		m_fRotation = value; 
		if(m_fRotation == 360.0f)
		{
			m_fRotation = 0.0f;
		}
	}
	float			GetRotation() { return m_fRotation; }
	
	void			SetTransMode(int value) { m_nTransMode = value; }
	int				GetTransMode() { return m_nTransMode; }
	
	void			SetArgb(int value) { m_nArgb = value; }
	int				GetArgb() { return m_nArgb; }
	
	void			SetScript(std::string &value) { m_Script = value; }
	std::string&	GetScript() { return m_Script; }

public:
	std::string		m_ResDir;			// 路径
	std::string		m_ResName;			// 文件名称
	int				m_ResType;			// 类型
	std::string		m_TargetName;		// 目标路径
	std::string		m_TargetAnimName;	// 目标路径
	std::string		m_Name;				// 名称
	int				m_nCurFrame;		// 当前序号
	float			m_fX;				// 位置
	float			m_fY;				// 位置
	float			m_fWidth;			// 大小
	float			m_fHeight;			// 大小
	float			m_fOriginX;			// 朝向
	float			m_fOriginY;			// 朝向
	float			m_fScaleX;			// 缩放
	float			m_fScaleY;			// 缩放
	float			m_fRotation;		// 旋转
	int				m_nTransMode;		// 朝向模式 1采用xy, 2 采用统一模式
	int				m_nArgb;			// 偏色
	LXAnimation*	m_pAnimation;
	
	std::string		m_Script;
};
