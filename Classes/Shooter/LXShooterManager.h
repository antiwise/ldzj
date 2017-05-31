#pragma once
#include "cocos2d.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// 弹幕管理器
// 负责弹幕数据的解析与加载

class LXShooter;
class LXShooterGroup;
class LXShooterManager
{
public:
	LXShooterManager();
	~LXShooterManager();

public:
	// 解析所有的弹幕文件
	void				ParseShootersData();		

	// 通过文件名获取弹幕信息
	LXShooterGroup*		GetShooterByName(std::string& name);

private:
	// 解析弹幕列表
	void				parseShooterFile(std::string path, std::string& filename);	

	// 解析弹幕文件
	void				parseShooterData(std::string& path, std::string& filename);	

private:
	
	// 对象池用于逻辑遍历
	std::map<std::string, LXShooterGroup*>		m_LXShootersMap;

};
LXShooterManager &ShooterManager();