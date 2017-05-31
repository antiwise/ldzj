#include "LXEffectManager.h"
#include "Common/FileUtils.h"

LXEffectManager &EffectManager()
{
	static LXEffectManager s;
	return s;
}

LXEffectManager::LXEffectManager()
{
	m_ResInfo1.push_back(EffectResInfo("baoxian", 8));
	m_ResInfo1.push_back(EffectResInfo("bombup", 1));
	m_ResInfo1.push_back(EffectResInfo("boom_m", 2));
	m_ResInfo1.push_back(EffectResInfo("boom_s", 10));
	m_ResInfo1.push_back(EffectResInfo("boss_fire1", 50));
	m_ResInfo1.push_back(EffectResInfo("boss_fire2", 50));
	m_ResInfo1.push_back(EffectResInfo("bossdeath", 1));
	m_ResInfo1.push_back(EffectResInfo("dianqiu", 1));
	m_ResInfo1.push_back(EffectResInfo("diaobao", 3));
	m_ResInfo1.push_back(EffectResInfo("diji06", 10));
	m_ResInfo1.push_back(EffectResInfo("fireball", 50));
	m_ResInfo1.push_back(EffectResInfo("fireball_boom", 30));
	m_ResInfo1.push_back(EffectResInfo("gaonengball", 20));
	m_ResInfo1.push_back(EffectResInfo("getcrystal", 20));
	m_ResInfo1.push_back(EffectResInfo("goods", 3));
	m_ResInfo1.push_back(EffectResInfo("hited", 6));
	m_ResInfo1.push_back(EffectResInfo("hurt", 5));
	m_ResInfo1.push_back(EffectResInfo("laser_fire", 12));
	m_ResInfo1.push_back(EffectResInfo("laser_hited", 10));
	m_ResInfo1.push_back(EffectResInfo("laser_hited_red", 10));
	m_ResInfo1.push_back(EffectResInfo("laser_hited_red2", 10));
	m_ResInfo1.push_back(EffectResInfo("lifeup", 1));
	m_ResInfo1.push_back(EffectResInfo("lightball1", 20));
	m_ResInfo1.push_back(EffectResInfo("lightball1_jz", 20));
	m_ResInfo1.push_back(EffectResInfo("lightball2", 20));
	m_ResInfo1.push_back(EffectResInfo("lightball2_jz", 20));
	m_ResInfo1.push_back(EffectResInfo("lock", 40));
	m_ResInfo1.push_back(EffectResInfo("mission", 1));
	m_ResInfo1.push_back(EffectResInfo("powermax", 1));
	m_ResInfo1.push_back(EffectResInfo("powerup", 5));
	m_ResInfo1.push_back(EffectResInfo("shieldup", 3));
	m_ResInfo1.push_back(EffectResInfo("super", 1));
	m_ResInfo1.push_back(EffectResInfo("super2", 1));
	m_ResInfo1.push_back(EffectResInfo("super3", 1));
	m_ResInfo1.push_back(EffectResInfo("tocrystal", 50));
	m_ResInfo1.push_back(EffectResInfo("transform", 1));
	m_ResInfo1.push_back(EffectResInfo("ui_warning", 1));
	m_ResInfo1.push_back(EffectResInfo("user_fire", 5));
	m_ResInfo1.push_back(EffectResInfo("user_firedian", 5));
	m_ResInfo1.push_back(EffectResInfo("user_hited", 1));
	m_ResInfo1.push_back(EffectResInfo("user3boom", 20));
	m_ResInfo1.push_back(EffectResInfo("useshield", 2));
	m_ResInfo1.push_back(EffectResInfo("victory", 1));
	m_ResInfo1.push_back(EffectResInfo("warning", 1));
	m_ResInfo1.push_back(EffectResInfo("transforming5", 1));
	m_ResInfo1.push_back(EffectResInfo("transform5_end", 1));
	m_ResInfo1.push_back(EffectResInfo("transform5_open", 1));
	m_ResInfo1.push_back(EffectResInfo("gaonengball2", 10));
	m_ResInfo1.push_back(EffectResInfo("boom_c", 10));
	m_ResInfo1.push_back(EffectResInfo("shield", 1));
}

LXEffectManager::~LXEffectManager() 
{
}

void LXEffectManager::LoadEffect(std::string& name, std::string& texturePath)
{
	ParticleEffect* particle = new ParticleEffect();
	particle->load(name, texturePath);
	m_ParticleTemplateMap[name] = particle;
}

void LXEffectManager::Tick(float fDelta)
{
	for(int i = 0; i < m_ParticleLogicVec.size(); i++)
	{
		if (m_ParticleLogicVec[i]->GetActiveNum() > 0)
		{
			m_ParticleLogicVec[i]->Tick(fDelta);
		}
	}
}

void LXEffectManager::LoadAllEffect()
{
	m_NameMap.clear();

	// 加载logo
	std::string strEffectName = "";
	std::string strEffectPath = "";

	// 加载其他
	for (int i = 0; i < m_ResInfo1.size(); i++)
	{
		strEffectName = "p0/" + m_ResInfo1[i].m_name + ".p";
		strEffectPath = "particleAtlas0";
		LoadEffect(strEffectName, strEffectPath);
		m_NameMap[m_ResInfo1[i].m_name] = strEffectName, strEffectPath;
	}

	
}

void LXEffectManager::PreLoad()
{
	LXFileUtils::AddSpriteFrameCache("particleAtlas0.plist", "particleAtlas0.png");
	LXFileUtils::AddSpriteFrameCache("sucai.plist", "sucai.png");
	InitAllEffect();
}

std::string	LXEffectManager::GetFullPath(std::string& name)
{
	std::map<std::string , std::string>::iterator it = m_NameMap.find(name);
	if (it != m_NameMap.end())
	{
		return it->second;
	}

	return name;
}

// 预创建特效
void LXEffectManager::InitAllEffect()
{
	for (int i = 0; i < m_ResInfo1.size(); i++)
	{
		std::string name = "p0/" + m_ResInfo1[i].m_name + ".p";
		int			num = m_ResInfo1[i].m_num;

		ParticleLogic *pLogic = new ParticleLogic();
		pLogic->InitEffectObject(name, num);

		m_ParticleLogicMap[name] = pLogic;
		m_ParticleLogicVec.push_back(pLogic);
	}

	for (int i = 0; i < m_ResInfo2.size(); i++)
	{
		std::string name = "p0_2/" + m_ResInfo2[i].m_name + ".p";
		int			num = m_ResInfo2[i].m_num;

		ParticleLogic *pLogic = new ParticleLogic();
		pLogic->InitEffectObject(name, num);

		m_ParticleLogicMap[name] = pLogic;
		m_ParticleLogicVec.push_back(pLogic);
	}

	for (int i = 0; i < m_ResInfo3.size(); i++)
	{
		std::string name = "p1/" + m_ResInfo3[i].m_name + ".p";
		int			num = m_ResInfo3[i].m_num;

		ParticleLogic *pLogic = new ParticleLogic();
		pLogic->InitEffectObject(name, num);

		m_ParticleLogicMap[name] = pLogic;
		m_ParticleLogicVec.push_back(pLogic);
	}
}

ParticleEffect*	LXEffectManager::CreateEffect(std::string& name)
{
	// 创建特效
	std::map<std::string, ParticleEffect*>::iterator itFind = m_ParticleTemplateMap.find(name);

	if (itFind != m_ParticleTemplateMap.end())
	{
		ParticleEffect *pEffect = new ParticleEffect(*itFind->second);
		return pEffect;
	}
	
	return nullptr;
}

ParticleEffect* LXEffectManager::CreateEffect(Node *pNode, std::string& name, float x, float y, int zorder, bool loop, bool needclean)
{
	std::map<std::string, ParticleLogic*>::iterator itFind = m_ParticleLogicMap.find(name);
	if (itFind != m_ParticleLogicMap.end())
	{
		ParticleEffect* pEffect = itFind->second->CreateEffect(pNode, x, y, zorder);
		if (pEffect )
		{
			pEffect->SetNeedClean(needclean);
			if (loop)
			{
				pEffect ->setContinuous();
			}
		}
		else
		{
			log("Erro!!!!!! LXEffectManager::CreateEffec name=%s is not found",name.c_str());
		}
		
		return pEffect;
	}

	return nullptr;
}

void LXEffectManager::DeleteEffect(ParticleEffect* pEffect)
{
	std::map<std::string, ParticleLogic*>::iterator itFind = m_ParticleLogicMap.find(pEffect->GetName());
	if (itFind != m_ParticleLogicMap.end())
	{
		itFind->second->DeleteEffect(pEffect);
	}
}

void	LXEffectManager::CleanEffects()
{
	std::map<std::string, ParticleLogic*>::iterator it = m_ParticleLogicMap.begin();
	while (it != m_ParticleLogicMap.end())
	{
		it->second->CleanEffect();
		it++;
	}
}