#pragma once
#include "cocos2d.h"
#include <vector>

USING_NS_CC;
enum
{
	animation_type_normal = 0,		// 正常播放
	animation_type_reverse,			// 倒叙播放
	animation_type_repeat,			// 重复播放
	animation_type_rrepeat,			// 倒叙重复播放
	animation_type_pingpong,		// 正常播放之后倒叙播放
	animation_type_random			// 随机播放
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

	// Tick逻辑
	void		Tick(float fDelta);

public:

	// 设置对象信息
	void		SetObject(LXObject *pObject) { m_pObject = pObject; }

	// 播放动画信息
	bool		PlayAnimation(std::string &name, const std::string &key, std::string &nameFix);

	// 设置动画类型 
	void		SetType(int value) { m_nType = value; }

	// 设置播放间隔
	void		SetDelta(float value) { m_fDelta = value; }

	// 增加关键帧
	void		AddFrame(int value);

	//  获取父节点信息
	Node*		GetParentNode() { return m_pParent; }  
	
	// 获取动画SIZE
	Size		GetAnimationSize();

	// 获取动画的锚点信息
	Point		GetAnchorPoint();

	// 设置颜色
	void		SetColor(Color3B color);

	Color3B		GetColor() { return m_Color; }

	// 获取动画元素
	Sprite*		GetRenderElement(std::string &path, std::string &key);

	// 设置回调状态
	void		SetCallBack(bool value) { m_bCallBack = value; }

	// 设置朝向
	void		SetFlip(bool value) { m_bFlip = value; }

private:

	// 初始化动画信息
	void		initAnimationInfo();

	// 初始化帧信息
	void		initFramesInfo();

	// 重置动画信息
	void		resetFrameInfo();

	// 计算关键帧信息
	int			calcFrame(float fDelta);

	// 计算动画是否播放结束
	bool		isAnimationEnd();

	// 逻辑成员
private:

	Node			*m_pParent;			// 动画父节点
	LXAnimation		*m_pAnimation;		// 动画信息
	LXObject		*m_pObject;			// 对象信息

	int				m_nType;			// 动画播放类型
	int				m_nFramePos;		// 关键帧位置
	int				m_nCurrentFrame;	// 当前关键帧
	int				m_nFrameNum;		// 关键帧数量
	int				m_FrameInfo[100];	// 关键帧信息
	float			m_fTime;			// 当前时间
	float			m_fDelta;			// 播放间隔
	std::string		m_NameFix;			// 资源名称修正
	bool			m_bFlip;			// 资源X反转
	bool			m_bCallBack;		// 是否回调

	// 渲染成员
private:
	Color3B			m_Color;				// 偏色处理
	int				m_nSpriteIndex;			// 当前占用的元素位置
	int				m_nAnimationNum;		// 动画元素缓存池数量
	AnimRenderElement*	m_SpritePool[100];	// 动画元素缓存池

};
