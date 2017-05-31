#pragma once
#include "ObjectEx/LXObject.h"

class LXPlane : public LXObject
{
public:
	LXPlane();
	~LXPlane();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();
	
	// 水晶掉落逻辑
	virtual void				DropCrystal();

	// 物品掉落逻辑
	virtual void				DropItem();

	// 水晶创建
	virtual void				CreatCrystal(bool DropGoodsCrystal,std::vector<int> vec);

	// 道具创建
	virtual void				CreatItem(int ItemId);

	// 死亡动画
	void						SetDieAnimate(const char* anim){ m_DieAnimate = anim; }
	std::string					GetDieAnimate(){ return m_DieAnimate; }

public:

	void			SetDropItemID( int nId ){ m_nDropItemID = nId; }
	int				GetDropItemID(){ return m_nDropItemID; }

	void			SetDropItemNum( int nNum ){ m_nDropItemNum = nNum; }
	int				GetDropItemNum(){ return m_nDropItemNum; }

	void			SetScore( int nScore ){ m_nScore = nScore; }
	int				GetScore(){ return m_nScore; }

private:
	int				m_nDropItemID;		// 掉落物品ID
	int				m_nDropItemNum;		// 掉落宝石数量ID
	int				m_nScore;			// 战机分数

	std::string		m_DieAnimate;		// 死亡动作
};
