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
	
	// 加载动画文件
	void LoadAnimation(std::string &fileName);

	// 卸载动画文件
	void UnLoadAnimation(std::string &fileName);

	// 获取KEY值	
	std::string GetKeyName(std::string &lh, const std::string &rh);

public:
	// 创建动画对象
	LXAnimationLogic*	CreateAnimationNode(CCNode *pNode, std::string &name, const std::string &key, std::string &nameFix, bool flip);
	
	// 获取动画信息
	LXAnimation*		GetAnimation(std::string &name);

private:

	// 读取Json文件
	bool readJson(const std::string &fileName, rapidjson::Document &doc);

	// 解析动画文件
	void parseAnimationFile(std::string &filename, rapidjson::Document &doc);

	// 判断动画文件是否已经加载
	bool isAnimationFileLoaded(std::string &fileName);

private:
	std::map<std::string, LXAnimation*> m_AnimationMap;
	std::list<std::string> m_AnimationFiles;
};

CAnimationManager &AnimationManager();
