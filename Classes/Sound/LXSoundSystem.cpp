#include "LXSoundSystem.h"
#include "DataBase/DataBase.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "GameDefine.h"

using namespace CocosDenshion;
USING_NS_CC;

CSoundSystem &SoundSystem()
{
	static CSoundSystem soundsys;
	return soundsys;
}

CSoundSystem::CSoundSystem()
{
	m_BID = -1;
	m_CachedBID = -1;
	m_BackgroundSoundState = true;
	m_EffectState = true;
	m_bShootEffect = false;
}

CSoundSystem::~CSoundSystem()
{
}

void CSoundSystem::Initial(void*)
{
	//m_BackgroundSoundState = CCUserDefault::getInstance()->getBoolForKey("BackgroundSound");
	//m_EffectState = CCUserDefault::getInstance()->getBoolForKey("EffectSound");
}

void CSoundSystem::Tick(void)
{
}

void CSoundSystem::Release(void)
{
}

const _DBC_SOUND* CSoundSystem::GetSoundDefine(int resID)
{
	DBC_DEFINEHANDLE(pDBC, DBC_SOUND);
	const _DBC_SOUND *pTheDefine = (const _DBC_SOUND*)pDBC->Search_Index_EQU(resID);
	return pTheDefine;
}

void CSoundSystem::preloadBackgroundMusic(int resID)
{
	if (m_BackgroundSoundState == false)
	{
		return;
	}
	
	const _DBC_SOUND *pTheDefine = GetSoundDefine(resID);

	if (pTheDefine)
	{
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pTheDefine->szPath);
	}
}

void CSoundSystem::playBackgroundMusic(int resID)
{
	if (m_BackgroundSoundState == false)
	{
		m_CachedBID = resID;
		return;
	}

	if(resID == m_BID)
	{
		return;
	}

	stopBackgroundMusic(true);

	const _DBC_SOUND *pTheDefine = GetSoundDefine(resID);
	if (pTheDefine)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(pTheDefine->szPath, pTheDefine->nLoop);
	}

	m_BID = resID;
	m_CachedBID = resID;
}

void CSoundSystem::stopBackgroundMusic(bool bReleaseData)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
	m_BID = -1;
}

void CSoundSystem::pauseBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void CSoundSystem::resumeBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void CSoundSystem::rewindBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
}

bool CSoundSystem::willPlayBackgroundMusic()
{
	if (m_BackgroundSoundState == false)
	{
		return false;
	}

	return SimpleAudioEngine::getInstance()->willPlayBackgroundMusic();
}

bool CSoundSystem::isBackgroundMusicPlaying()
{
	return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

float CSoundSystem::getBackgroundMusicVolume()
{
	return SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

void CSoundSystem::setBackgroundMusicVolume(float volume)
{
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

float CSoundSystem::getEffectsVolume()
{
	return SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void CSoundSystem::setEffectsVolume(float volume)
{
	SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

unsigned int CSoundSystem::playEffect(int resID)
{
	if (m_EffectState == false)
	{
		return -1; 
	}

	if (resID == Mic_bullet1)
	{
		if (m_bShootEffect)
		{
			return -1;
		}

		m_bShootEffect = true;
	}

	//bool bValue = cocos2d::CCUserDefaults::getInstance()->getBoolForKey("loading");
	//if (bValue)
	//{
	//	return -1;
	//}
	
	const _DBC_SOUND *pTheDefine = GetSoundDefine(resID);
	if (pTheDefine)
	{
		log("music :=%s",pTheDefine->szPath);
		return SimpleAudioEngine::getInstance()->playEffect(pTheDefine->szPath, pTheDefine->nLoop);
	}

	return -1;
}

void CSoundSystem::pauseEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);
}

void CSoundSystem::pauseAllEffects()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void CSoundSystem::resumeEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);
}

void CSoundSystem::resumeAllEffects()
{
	SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void CSoundSystem::stopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}

void CSoundSystem::stopAllEffects()
{
	m_bShootEffect = false;
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

void CSoundSystem::preloadEffect(int resID)
{
	if (m_EffectState == false)
	{
		return; 
	}

	const _DBC_SOUND *pTheDefine = GetSoundDefine(resID);
	if (pTheDefine)
	{
		return SimpleAudioEngine::getInstance()->preloadEffect(pTheDefine->szPath);
	}
}

void CSoundSystem::unloadEffect(int resID)
{
	const _DBC_SOUND *pTheDefine = GetSoundDefine(resID);
	if (pTheDefine)
	{
		return SimpleAudioEngine::getInstance()->unloadEffect(pTheDefine->szPath);
	}
}

void CSoundSystem::setBackgroundMusicState(bool bState)
{
	if (m_BackgroundSoundState != bState)
	{
		m_BackgroundSoundState = bState;

		if ( bState)
		{
			playBackgroundMusic(m_CachedBID);
		}
		else
		{
			stopBackgroundMusic(true);
		}

		cocos2d::CCUserDefault::getInstance()->setBoolForKey("BackgroundSound", bState);
		cocos2d::CCUserDefault::getInstance()->flush();
	}
}

void CSoundSystem::setEffectState(bool bState)
{
	if (m_EffectState!= bState)
	{
		m_EffectState = bState;
		cocos2d::CCUserDefault::getInstance()->setBoolForKey("EffectSound", bState);
		cocos2d::CCUserDefault::getInstance()->flush();
	}
}

//void CSoundSystem::unloadEffectNotInScene(int nSceneID)
//{
//	//
//	const int AllSceneEffect = 4;//各场景通用类型,不卸载;
//	DBC_DEFINEHANDLE(pDBC, DBC_SOUND);
//	const int nCount = pDBC->GetRecordsNum();
//	for (int i=0; i< nCount; i++)
//	{
//		const _DBC_SOUND *pTheDefine = (const _DBC_SOUND*)pDBC->Search_LineNum_EQU(i);
//
//		if (pTheDefine && pTheDefine->nLoop == 0 && pTheDefine->nSenceID != nSceneID && pTheDefine->nSenceID != AllSceneEffect)
//		{
//			//SimpleAudioEngine::getInstance()->unloadEffect(pTheDefine->szPath);
//			//CCLOG("unloadEffectNotInScene:%d path=%s",nSceneID,pTheDefine->szPath);
//		}	
//	}
//}