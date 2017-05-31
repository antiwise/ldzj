#include "LXShooter.h"
#include "LXShooterLogic.h"
#include "ObjectEx/LXObject.h"

LXShooter::LXShooter()
	:m_nType(0),			
	 m_nResId(0),			
	 m_fOffX(0.0f),			
	 m_fOffY(0.0f),			
	 m_fSpeed(0.0f),			
	 m_fDegree(0.0f),			
	 m_fDuration(0.0f),		
	 m_fDelay(0.0f),			
	 m_nGroup(0),			
	 m_nNum(0),				
	 m_fWaitTime(0.0f),		
	 m_fGroupWaitTime(0.0f),	
	 m_fDegreeDelta(0.0f),		
	 m_fGroupDegreeDelta(0.0f),
	 m_fSpeedDelta(0.0f),		
	 m_bCanThrough(false),		
	 m_fBurstTime(0.0f),		
	 m_fMinSpeed(0.0f),		
	 m_fMaxSpeed(0.0f),		
	 m_fDeltaSpeed(0.0f),		
	 m_fMaxDeltaDegree(0.0f),	
	 m_bDeltaSpeedChange(false)
{
}

LXShooter::~LXShooter() 
{
}

LXShooterGroup::LXShooterGroup()
{

}

LXShooterGroup::~LXShooterGroup()
{
	// 清理所有武器
	for (int i = 0; i < m_Shooters.size(); i++)
	{
		delete m_Shooters[i];
	}

	m_Shooters.clear();
}

void LXShooterGroup::AddShooter(LXShooter* pShooter)
{
	m_Shooters.push_back(pShooter);
}

void LXShooterGroup::InitShooterLogic(LXObject *pObject, std::vector<LXShooterLogic*> &vec)
{
	for (int i = 0; i < m_Shooters.size(); i++)
	{
		LXShooterLogic *pLogic = new LXShooterLogic();
		pLogic->SetParent(pObject);
		pLogic->SetShooter(m_Shooters[i]);
		pLogic->SetAnchorPointX(m_Shooters[i]->GetOffX());
		pLogic->SetAnchorPointY(m_Shooters[i]->GetOffY());
		vec.push_back(pLogic);
	}
}