/****************************************\
*										*
* 			   ��Ч����					*
*										*
\****************************************/
#pragma once

struct _DBC_SOUND;
class CSoundSystem
{
public:
	// ��������״̬
	void setBackgroundMusicState(bool bState);
	bool getBackgroundMusicState() { return m_BackgroundSoundState; }
	// ��Ч״̬
	void setEffectState(bool bState);
	bool getEffectState() { return m_EffectState; }
	// Ԥ������Ч��Դ
	void preloadBackgroundMusic(int resID);
	// ���ű�������
	void playBackgroundMusic(int resID);
    // ֹͣ���ű�������
	void stopBackgroundMusic(bool bReleaseData);
    // ��ͣ��������
    void pauseBackgroundMusic();
	// �ָ���������
    void resumeBackgroundMusic();
	// ���¿�ʼ���ű�������
    void rewindBackgroundMusic();
	// �Ƿ񼴽����ű�������
    bool willPlayBackgroundMusic();
	// ���������Ƿ񲥷�
    bool isBackgroundMusicPlaying();
	// ������������
    float getBackgroundMusicVolume();
	// ���ñ�����������
    void setBackgroundMusicVolume(float volume);
	// ��Ч����
    float getEffectsVolume();
	// ������Ч����
    void setEffectsVolume(float volume);
	// ������Ч
    unsigned int playEffect(int resID);
	// ��ͣ��Ч
    void pauseEffect(unsigned int nSoundId);
	// ��ͣ������Ч
    void pauseAllEffects();
	// �ָ���Ч
    void resumeEffect(unsigned int nSoundId);
	// �ָ�����������Ч
    void resumeAllEffects();
	// ֹͣ��Ч
    void stopEffect(unsigned int nSoundId);
	// ֹͣ������Ч
    void stopAllEffects();
	// Ԥ������Ч
    void preloadEffect(int resID);
	// ж����Ч
    void unloadEffect(int resID);
	//// ��������Ч������
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
	int					m_CachedBID;			//����ı�������ID
	int					m_BID;					//��ǰ���ŵı�������ID
	bool				m_BackgroundSoundState;	//��Ч״̬
	bool				m_EffectState;			//��Ч״̬
	bool				m_bShootEffect;			//���������Ч

protected:

};

CSoundSystem &SoundSystem();