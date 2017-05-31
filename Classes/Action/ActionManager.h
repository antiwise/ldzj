#pragma once
#include <map>
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class LXAction;
//管理Action类 通过解析文本来管理播放动画
class CActionManager
{
public:
	CActionManager(void);
	~CActionManager(void);

public:
	
	// 加载动画文件
	void				LoadAction(const std::string &fileName);

	// 卸载动画文件
	void				UnLoadAction(const std::string &fileName);

	// 执行动画
	void				DoAction(Node* pNode, const std::string &actionName, int tag);

	//add by max
	ActionInterval*				getAct(const std::string &actionName, int tag);

private:

	// 读取Json文件
	bool				readJson(const std::string &fileName, rapidjson::Document &doc);

	// 解析动画文件
	void				parseActionFile(const std::string &filename, rapidjson::Document &doc);

private:
	std::map<std::string, LXAction*> m_ActionMap;

};

CActionManager &LXActionManager();
