#include "ParticleEffect.h"
#include "Common/FileUtils.h"

ParticleEffect::ParticleEffect()
	: m_pParent(nullptr),
	m_nTag(0),
	m_nZOrder(0),
	m_bNeedClean(true),
	m_bActive(false),
	m_bIsLoop(false)
{
}

ParticleEffect::ParticleEffect(const ParticleEffect &effect)
{
	for (int i = 0, n = effect.emitters.size(); i < n; i++)
	{
		emitters.push_back(new ParticleEmitter(*effect.emitters[i]));
	}

	bounds			= effect.bounds;
	m_pParent		= effect.m_pParent;
	m_nTag			= effect.m_nTag;
	m_nZOrder		= effect.m_nZOrder;
	m_Name			= effect.m_Name;
	m_bNeedClean	= effect.m_bNeedClean;
	m_bActive		= effect.m_bActive;
	m_bIsLoop		= effect.m_bIsLoop;
}

ParticleEffect::~ParticleEffect ()
{

}

ParticleEmitter* ParticleEffect::findEmitter (std::string& name)
{
	for (int i = 0; i < emitters.size(); i++) {
		ParticleEmitter* emitter = emitters[i];
		if (emitter->getName() == name)
		{
			return emitter;
		}
	}
	return nullptr;
}

void ParticleEffect::load (std::string& filename, std::string& texturePath) 
{
	m_Name = filename;
	std::string effectFilePath = filename;
	LXFileUtils::GetEffectFullPath(effectFilePath);
	std::istringstream ifs(FileUtils::getInstance()->getStringFromFile(effectFilePath));
	if (!ifs)
	{
		log("Cannot open file [%s]",filename.c_str());
		return;
	}

	int maxchars = 8192;				// Alloc enough size.
	std::vector<char> buf(maxchars);	// Alloc enough size.
	while (ifs.peek() != -1)
	{
		ParticleEmitter* emitter = new ParticleEmitter(m_pParent, ifs, texturePath, m_nZOrder);
		ifs.getline(&buf[0], maxchars);
		ifs.getline(&buf[0], maxchars);
		std::string strImgPath = &buf[0];
		emitter->setImagePath(strImgPath);
		emitters.push_back(emitter);

		if(ifs.peek() == -1)
		{
			break;
		}

		ifs.getline(&buf[0], maxchars);
		ifs.getline(&buf[0], maxchars);
	}
}

void ParticleEffect::ReleaseRenderInfo()
{
	// 清理渲染对象
	for (int i = 0; i < emitters.size(); i++)
		emitters[i]->releaseRenderInfo();
}

void ParticleEffect::Release()
{
	// 清理渲染对象
	for (int i = 0; i < emitters.size(); i++)
		emitters[i]->release();
}