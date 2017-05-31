#include "ParticleLogic.h"
#include "Common/FileUtils.h"
#include "LXEffectManager.h"

ParticleLogic::ParticleLogic ()
	: m_bLoop(false),
	m_nNum(1),
	m_nActiveNum(0)
{
	memset(m_EffectList,0,sizeof(m_EffectList));
}

ParticleLogic::~ParticleLogic ()
{

}

void ParticleLogic::InitEffectObject(std::string& name, int num)
{
	// 保存对象路径
	m_Name = name;
	
	// 数量
	m_nNum = num; 

	// 创建缓存对象
	for (int i = 0; i < m_nNum; i++)
	{
		m_EffectList[i] = EffectManager().CreateEffect(name);
	}
}

ParticleEffect*	ParticleLogic::CreateEffect(Node *pNode, float x, float y, int zorder)
{
	ParticleEffect*	effect = nullptr;
	// 获取一个特效对象
	for (int i = 0; i < m_nNum; i++)
	{
		if (m_EffectList[i]->IsActive() == false)
		{
			effect = m_EffectList[i];
			
			effect->SetActive(true);
			effect->setPosition(x, y);
			effect->SetZOrder(zorder);
			effect->reset();
			if(effect->GetParent() != pNode)
			{
				effect->SetParent(pNode);
			}
			m_nActiveNum++;
			break;
		}
	}

	return effect;
}

void ParticleLogic::DeleteEffect(ParticleEffect* pEffect)
{
	if(pEffect)
	{
		for (int i = 0; i < m_nNum; i++)
		{
			if(pEffect == m_EffectList[i] && m_EffectList[i]->IsActive())
			{
				m_EffectList[i]->Release();
				m_EffectList[i]->SetActive(false);
				m_nActiveNum--;
				break;
			}
		}
	}
}

void ParticleLogic::CleanEffect()
{
	for (int i = 0; i < m_nNum; i++)
	{
		if (m_EffectList[i]->IsNeedClean())
		{
			// 清理渲染资源
			m_EffectList[i]->ReleaseRenderInfo();
			m_EffectList[i]->SetActive(false);
		}
	}

	m_nActiveNum = 0;
}

void ParticleLogic::Tick(float fDelta)
{
	int leftnum = m_nActiveNum;
	for (int i = 0; i < m_nNum; i++)
	{
		if (m_EffectList[i]->IsActive())
		{
			m_EffectList[i]->update(fDelta);
			if (m_EffectList[i]->isComplete())
			{
				if (m_EffectList[i]->IsLoop())
				{
					m_EffectList[i]->reset();
				}
				else
				{
					DeleteEffect(m_EffectList[i]);
				}
			}
			leftnum--;
			if (leftnum == 0)
			{
				break;
			}
		}
	}
}

void ParticleLogic::SetEffectContinues()
{
	for (int i = 0; i < m_nNum; i++)
	{
		m_EffectList[i]->setContinuous();
	}
}