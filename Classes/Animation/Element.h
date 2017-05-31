#pragma once
#include <string>
#include "cocos2d.h"

enum
{
	resource_type_pic = 0,		// ͼƬ
	resource_type_child			// �Ӷ���
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
	std::string		m_ResDir;			// ·��
	std::string		m_ResName;			// �ļ�����
	int				m_ResType;			// ����
	std::string		m_TargetName;		// Ŀ��·��
	std::string		m_TargetAnimName;	// Ŀ��·��
	std::string		m_Name;				// ����
	int				m_nCurFrame;		// ��ǰ���
	float			m_fX;				// λ��
	float			m_fY;				// λ��
	float			m_fWidth;			// ��С
	float			m_fHeight;			// ��С
	float			m_fOriginX;			// ����
	float			m_fOriginY;			// ����
	float			m_fScaleX;			// ����
	float			m_fScaleY;			// ����
	float			m_fRotation;		// ��ת
	int				m_nTransMode;		// ����ģʽ 1����xy, 2 ����ͳһģʽ
	int				m_nArgb;			// ƫɫ
	LXAnimation*	m_pAnimation;
	
	std::string		m_Script;
};
