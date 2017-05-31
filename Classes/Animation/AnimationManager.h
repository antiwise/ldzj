#pragma once
#include <map>
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class LXAnimation;
class LXAnimationLogic;
class CAnimationManager
{
public:
	CAnimationManager(void);
	~CAnimationManager(void);

public:
	
	// ���ض����ļ�
	void LoadAnimation(std::string &fileName);

	// ж�ض����ļ�
	void UnLoadAnimation(std::string &fileName);

	// ��ȡKEYֵ	
	std::string GetKeyName(std::string &lh, const std::string &rh);

public:
	// ������������
	LXAnimationLogic*	CreateAnimationNode(CCNode *pNode, std::string &name, const std::string &key, std::string &nameFix, bool flip);
	
	// ��ȡ������Ϣ
	LXAnimation*		GetAnimation(std::string &name);

private:

	// ��ȡJson�ļ�
	bool readJson(const std::string &fileName, rapidjson::Document &doc);

	// ���������ļ�
	void parseAnimationFile(std::string &filename, rapidjson::Document &doc);

	// �ж϶����ļ��Ƿ��Ѿ�����
	bool isAnimationFileLoaded(std::string &fileName);

private:
	std::map<std::string, LXAnimation*> m_AnimationMap;
	std::list<std::string> m_AnimationFiles;
};

CAnimationManager &AnimationManager();
