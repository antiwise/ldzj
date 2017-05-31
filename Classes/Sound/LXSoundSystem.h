/****************************************\
*										*
* 			   音效管理					*
*										*
\****************************************/
#pragma once

struct _DBC_SOUND;
class CSoundSystem
{
public:
	// 背景音乐状态
	void setBackgroundMusicState(bool bState);
	bool getBackgroundMusicState() { return m_BackgroundSoundState; }
	// 音效状态
	void setEffectState(bool bState);
	bool getEffectState() { return m_EffectState; }
	// 预加载音效资源
	void preloadBackgroundMusic(int resID);
	// 播放背景音乐
	void playBackgroundMusic(int resID);
    // 停止播放背景音乐
	void stopBackgroundMusic(bool bReleaseData);
    // 暂停背景音乐
    void pauseBackgroundMusic();
	// 恢复背景音乐
    void resumeBackgroundMusic();
	// 重新开始播放背景音乐
    void rewindBackgroundMusic();
	// 是否即将播放背景音乐
    bool willPlayBackgroundMusic();
	// 背景音乐是否播放
    bool isBackgroundMusicPlaying();
	// 背景音乐音量
    float getBackgroundMusicVolume();
	// 设置背景音乐音量
    void setBackgroundMusicVolume(float volume);
	// 音效音量
    float getEffectsVolume();
	// 设置音效音量
    void setEffectsVolume(float volume);
	// 播放音效
    unsigned int playEffect(int resID);
	// 暂停音效
    void pauseEffect(unsigned int nSoundId);
	// 暂停所有音效
    void pauseAllEffects();
	// 恢复音效
    void resumeEffect(unsigned int nSoundId);
	// 恢复播放所有音效
    void resumeAllEffects();
	// 停止音效
    void stopEffect(unsigned int nSoundId);
	// 停止所有音效
    void stopAllEffects();
	// 预加载音效
    void preloadEffect(int resID);
	// 卸载音效
    void unloadEffect(int resID);
	//// 场景中无效的音乐
	//void unloadEffectNotInScene(int nSceneID);

public:
	CSoundSystem();
	virtual ~CSoundSystem();

public:
	virtual void		Initial(void*);
	virtual void		Release(void);
	virtual void		Tick(void);

protected:
	const _DBC_SOUND*	GetSoundDefine(int resID);

protected:
	int					m_CachedBID;			//缓存的背景音乐ID
	int					m_BID;					//当前播放的背景音乐ID
	bool				m_BackgroundSoundState;	//音效状态
	bool				m_EffectState;			//音效状态
	bool				m_bShootEffect;			//播放射击音效

protected:

};

CSoundSystem &SoundSystem();