#pragma once
#include "cocos2d.h"
#include "ParticleEmitter.h"

USING_NS_CC;

class ParticleEffect 
{
public:
	ParticleEffect ();
	ParticleEffect(const ParticleEffect &effect);
	~ParticleEffect ();

public:

	void start () {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->start();
	}

	void reset () {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->reset();
	}

	void update (float delta) {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->update(delta);
	}
	
	void allowCompletion () {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->AllowCompletion();
	}

	bool isComplete () {
		for (int i = 0; i < emitters.size(); i++) {
			ParticleEmitter* emitter = emitters[i];
			if (!emitter->isComplete()) return false;
		}
		return true;
	}

	void setDuration (int duration) {
		for (int i = 0; i < emitters.size(); i++) {
			ParticleEmitter* emitter = emitters[i];
			emitter->setContinuous(false);
			emitter->setDurationValue(duration);
			emitter->setDurationTimer(0);
		}
	}

	void setPosition (float x, float y) {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->setPosition(x, y);
	}

	void setFlip (bool flipX, bool flipY) {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->setFlip(flipX, flipY);
	}

	void flipY () {
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->FlipY();
	}

	void setContinuous()
	{
		m_bIsLoop = true;

		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->setContinuous(true);
	}
	
	const std::vector<ParticleEmitter*> getEmitters () {
		return emitters;
	}

	ParticleEmitter* findEmitter (std::string& name);

	void load (std::string& filename, std::string& texturePath);

	void		SetTag(int value) { m_nTag = value; }
	int			GetTag() { return m_nTag; }
	
	void		SetZOrder(int value)
	{
		m_nZOrder = value;
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->SetZOrder(value);
	}

	int			GetZOrder() { return m_nZOrder; }

	void		SetParent(Node* pNode)  {
		m_pParent = pNode;
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->SetParent(m_pParent);
	}

	Node*		GetParent() { return m_pParent; }

	void		SetNeedClean(bool needclean) { m_bNeedClean = needclean;}
	bool		IsNeedClean() { return m_bNeedClean;}

	// 清理渲染对象
	void		ReleaseRenderInfo();

	// 回收
	void		Release();

	// 循环
	void		SetIsLoop(bool _isLoop){ m_bIsLoop = _isLoop; }
	bool		IsLoop(){ return m_bIsLoop; }

	// 获取特效名称
	std::string GetName() { return m_Name; }

	bool		IsActive() { return m_bActive; }
	void		SetActive(bool value) { m_bActive = value; m_pParent = nullptr; }

	void		setVisible(bool value)
	{
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->setVisible(value);
	}

	void		SetScale(float value)
	{
		for (int i = 0; i < emitters.size(); i++)
			emitters[i]->SetScale(value);
	}

protected:
	std::vector<ParticleEmitter*> emitters;
	Rect			bounds;
	Node*			m_pParent;
	int				m_nTag;
	int				m_nZOrder;
	std::string		m_Name;
	bool			m_bNeedClean;
	bool			m_bActive;
	bool			m_bIsLoop;
};
