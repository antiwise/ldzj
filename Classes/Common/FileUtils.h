#pragma once
#include <string>
#include <iostream>  
#include "cocos2d.h"  
using namespace cocos2d;  

class LXFileUtils
{

public:

	/** 读取本地文件，返回数据 */  
	static std::string GetFileByName(std::string pFileName);  

	/** 储存内容到文件 */  
	static bool SaveFile(const char* pContent,std::string pFileName);  

	static int	ConvertStringToVector(const char *strSource, std::vector< std::string >& vRet, const char *szKey= "\\/", bool bOneOfKey = true, bool bIgnoreEmpty = true);


	// 获取特效路径
	static void GetEffectFullPath(std::string &key);

	// 获取纹理路径
	static void GetTexturePath(std::string &key);
	static void GetTexturePathLeijie(std::string &key);
	// 获取动画路径
	static void GetAnimationFullPath(std::string &key);

	// 获取动作路径
	static void GetActionFullPath(std::string &key);

	// 获取地图路径
	static void GetMapFullPath(std::string &key);

	// 获取战斗背景路径
	static void GetMapBgFullPath(std::string &key);

	// 获取弹幕路径
	static void GetShooterFullPath(std::string &key);

	// 获取特效图片路径
	static void GetParticleFullPath(std::string &key);

	//血条图片路径
	static void GetBloodBarFullPath(std::string &key);

	// UI路径文件路径
	static void Get_UI_Json_FullPath(std::string &key );

	static void AddSpriteFrameCache(const std::string &plist, const std::string &png);

	static void encrypt(char *x, size_t xlen, char* KEY, size_t BeginPlace);
};
