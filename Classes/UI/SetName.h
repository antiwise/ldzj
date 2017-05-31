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

	// 点击回调
	void	textFieldEvent(Ref* pSender, TextField::EventType type);

	// 随机取名回调
	void	RandNaneTouchEvent(Ref* pSender, Widget::TouchEventType type);
	
	// 确定回调
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);

// protected:
// 	void				onDraw(const Mat4 &transform);
// 
// 	virtual void		draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
	bool	HasForbiddenWords(const std::string& strIn);

private:

	OpenSetNameType		m_OpenType;		// 打开类型

	TextField*			m_TextField;	// 输入框

	ImageView*			m_SetNameBackGround;		// 设置名称
	//CustomCommand		m_CustomCommand;			// 自定义绘制
	Sprite*				m_SpriteWarning;	//敏感词提示
};
