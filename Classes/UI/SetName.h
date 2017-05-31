#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

enum OpenSetNameType
{
	OpenSetNameType_Ranking,
	OpenSetNameType_Battle,
};

class SetName : public Layer
{
public:
	SetName(void);
	~SetName(void);
	
	CREATE_FUNC(SetName);

	virtual bool init();
	

public:

	void	Init( OpenSetNameType eType );

	// ����ص�
	void	textFieldEvent(Ref* pSender, TextField::EventType type);

	// ���ȡ���ص�
	void	RandNaneTouchEvent(Ref* pSender, Widget::TouchEventType type);
	
	// ȷ���ص�
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);

// protected:
// 	void				onDraw(const Mat4 &transform);
// 
// 	virtual void		draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
	bool	HasForbiddenWords(const std::string& strIn);

private:

	OpenSetNameType		m_OpenType;		// ������

	TextField*			m_TextField;	// �����

	ImageView*			m_SetNameBackGround;		// ��������
	//CustomCommand		m_CustomCommand;			// �Զ������
	Sprite*				m_SpriteWarning;	//���д���ʾ
};
