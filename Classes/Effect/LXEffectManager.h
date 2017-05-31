#pragma once
#include <vector>
#include "cocos2d.h"
#include "Effect/ParticleEffect.h"
#include "ParticleLogic.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// 特效管理类

struct EffectResInfo
{
public:
	EffectResInfo(std::string name, int num)
		: m_name(name),
		m_num(num)
	{
	}

	std::string m_name;
	int			m_num;
};

class LXEffectManager
{
public:
	LXEffectManager();
	~LXEffectManager();

public:
	// Tick
	void				Tick(float fDelta);	

	// 加载全部特效
	void				LoadAllEffect();

	// 加载特效
	void				LoadEffect(std::string& name, std::string& texturePath);

	// 预创建特效
	void				InitAllEffect();

	// 预加载
	void				PreLoad();

public:
	// 获取粒子特效
	ParticleEffect*		CreateEffect(Node *pNode, std::string &name, float x, float y, int zorder, bool loop = false, bool needclean = true);
	
	// 创建粒子
	ParticleEffect*		CreateEffect(std::string &name);

	// 删除粒子
	void				DeleteEffect(ParticleEffect* pEffect);

	// 清理所有带标记的粒子
	void				CleanEffects();

	// 获取完整路径
	std::string			GetFullPath(std::string& name);

private:

	// 获取空闲对象
	ParticleEffect*		getFreeObject(int type);
	
	std::map<std::string , std::string>		m_NameMap;						// 名称映射
	std::map<std::string, ParticleLogic*>	m_ParticleLogicMap;				// 粒子逻辑
	std::vector<ParticleLogic*>				m_ParticleLogicVec;				// 粒子逻辑
	std::map<std::string, ParticleEffect*>	m_ParticleTemplateMap;			// 粒子模版

	std::vector<EffectResInfo>				m_ResInfo1;						// 特效信息
	std::vector<EffectResInfo>				m_ResInfo2;						// 特效信息
	std::vector<EffectResInfo>				m_ResInfo3;						// 特效信息
};

LXEffectManager &EffectManager();