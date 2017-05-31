#pragma once
#include "cocos2d.h"
#include <vector>

USING_NS_CC;
enum
{
	animation_type_normal = 0,		// ��������
	animation_type_reverse,			// ���𲥷�
	animation_type_repeat,			// �ظ�����
	animation_type_rrepeat,			// �����ظ�����
	animation_type_pingpong,		// ��������֮���𲥷�
	animation_type_random			// �������
};

struct AnimRenderElement
{
	AnimRenderElement()
	{
		pSprite = nullptr;
		bState = false;
	}

	Sprite		*pSprite;
	std::string key;
	bool		bState;
};

class LXAnimation;
class LXObject;
class LXAnimationLogic
{
public:
	LXAnimationLogic(Node *pParent, LXAnimation* pAnimation, std::string &nameFix, bool flip);
	~LXAnimationLogic();

	// Tick�߼�
	void		Tick(float fDelta);

public:

	// ���ö�����Ϣ
	void		SetObject(LXObject *pObject) { m_pObject = pObject; }

	// ���Ŷ�����Ϣ
	bool		PlayAnimation(std::string &name, const std::string &key, std::string &nameFix);

	// ���ö������� 
	void		SetType(int value) { m_nType = value; }

	// ���ò��ż��
	void		SetDelta(float value) { m_fDelta = value; }

	// ���ӹؼ�֡
	void		AddFrame(int value);

	//  ��ȡ���ڵ���Ϣ
	Node*		GetParentNode() { return m_pParent; }  
	
	// ��ȡ����SIZE
	Size		GetAnimationSize();

	// ��ȡ������ê����Ϣ
	Point		GetAnchorPoint();

	// ������ɫ
	void		SetColor(Color3B color);

	Color3B		GetColor() { return m_Color; }

	// ��ȡ����Ԫ��
	Sprite*		GetRenderElement(std::string &path, std::string &key);

	// ���ûص�״̬
	void		SetCallBack(bool value) { m_bCallBack = value; }

	// ���ó���
	void		SetFlip(bool value) { m_bFlip = value; }

private:

	// ��ʼ��������Ϣ
	void		initAnimationInfo();

	// ��ʼ��֡��Ϣ
	void		initFramesInfo();

	// ���ö�����Ϣ
	void		resetFrameInfo();

	// ����ؼ�֡��Ϣ
	int			calcFrame(float fDelta);

	// ���㶯���Ƿ񲥷Ž���
	bool		isAnimationEnd();

	// �߼���Ա
private:

	Node			*m_pParent;			// �������ڵ�
	LXAnimation		*m_pAnimation;		// ������Ϣ
	LXObject		*m_pObject;			// ������Ϣ

	int				m_nType;			// ������������
	int				m_nFramePos;		// �ؼ�֡λ��
	int				m_nCurrentFrame;	// ��ǰ�ؼ�֡
	int				m_nFrameNum;		// �ؼ�֡����
	int				m_FrameInfo[100];	// �ؼ�֡��Ϣ
	float			m_fTime;			// ��ǰʱ��
	float			m_fDelta;			// ���ż��
	std::string		m_NameFix;			// ��Դ��������
	bool			m_bFlip;			// ��ԴX��ת
	bool			m_bCallBack;		// �Ƿ�ص�

	// ��Ⱦ��Ա
private:
	Color3B			m_Color;				// ƫɫ����
	int				m_nSpriteIndex;			// ��ǰռ�õ�Ԫ��λ��
	int				m_nAnimationNum;		// ����Ԫ�ػ��������
	AnimRenderElement*	m_SpritePool[100];	// ����Ԫ�ػ����

};
