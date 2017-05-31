#pragma once
#include <vector>
#include "cocos2d.h"
#include "Effect/ParticleEffect.h"
#include "ParticleLogic.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// ��Ч������

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

	// ����ȫ����Ч
	void				LoadAllEffect();

	// ������Ч
	void				LoadEffect(std::string& name, std::string& texturePath);

	// Ԥ������Ч
	void				InitAllEffect();

	// Ԥ����
	void				PreLoad();

public:
	// ��ȡ������Ч
	ParticleEffect*		CreateEffect(Node *pNode, std::string &name, float x, float y, int zorder, bool loop = false, bool needclean = true);
	
	// ��������
	ParticleEffect*		CreateEffect(std::string &name);

	// ɾ������
	void				DeleteEffect(ParticleEffect* pEffect);

	// �������д���ǵ�����
	void				CleanEffects();

	// ��ȡ����·��
	std::string			GetFullPath(std::string& name);

private:

	// ��ȡ���ж���
	ParticleEffect*		getFreeObject(int type);
	
	std::map<std::string , std::string>		m_NameMap;						// ����ӳ��
	std::map<std::string, ParticleLogic*>	m_ParticleLogicMap;				// �����߼�
	std::vector<ParticleLogic*>				m_ParticleLogicVec;				// �����߼�
	std::map<std::string, ParticleEffect*>	m_ParticleTemplateMap;			// ����ģ��

	std::vector<EffectResInfo>				m_ResInfo1;						// ��Ч��Ϣ
	std::vector<EffectResInfo>				m_ResInfo2;						// ��Ч��Ϣ
	std::vector<EffectResInfo>				m_ResInfo3;						// ��Ч��Ϣ
};

LXEffectManager &EffectManager();