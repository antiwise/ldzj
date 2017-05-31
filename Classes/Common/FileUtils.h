#pragma once
#include <string>
#include <iostream>  
#include "cocos2d.h"  
using namespace cocos2d;  

class LXFileUtils
{

public:

	/** ��ȡ�����ļ����������� */  
	static std::string GetFileByName(std::string pFileName);  

	/** �������ݵ��ļ� */  
	static bool SaveFile(const char* pContent,std::string pFileName);  

	static int	ConvertStringToVector(const char *strSource, std::vector< std::string >& vRet, const char *szKey= "\\/", bool bOneOfKey = true, bool bIgnoreEmpty = true);


	// ��ȡ��Ч·��
	static void GetEffectFullPath(std::string &key);

	// ��ȡ����·��
	static void GetTexturePath(std::string &key);
	static void GetTexturePathLeijie(std::string &key);
	// ��ȡ����·��
	static void GetAnimationFullPath(std::string &key);

	// ��ȡ����·��
	static void GetActionFullPath(std::string &key);

	// ��ȡ��ͼ·��
	static void GetMapFullPath(std::string &key);

	// ��ȡս������·��
	static void GetMapBgFullPath(std::string &key);

	// ��ȡ��Ļ·��
	static void GetShooterFullPath(std::string &key);

	// ��ȡ��ЧͼƬ·��
	static void GetParticleFullPath(std::string &key);

	//Ѫ��ͼƬ·��
	static void GetBloodBarFullPath(std::string &key);

	// UI·���ļ�·��
	static void Get_UI_Json_FullPath(std::string &key );

	static void AddSpriteFrameCache(const std::string &plist, const std::string &png);

	static void encrypt(char *x, size_t xlen, char* KEY, size_t BeginPlace);
};
