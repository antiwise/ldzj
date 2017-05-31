#pragma once

#include <vector>
#include <string>
#include <map>
#include "cocos2d.h"
using namespace std;

USING_NS_CC;
class CUtils
{
public:
	void				createDir(const char* szPath);
	void				copyDataFile(const char* pFileName, const char* pTargetPath);

	// 字符串分割
	int					SplitPairStringToVector(const std::string &strSource, std::vector<std::pair<int,int> > &vect, const char* split, const char* pairSplit );
	int					SplitPairStringToVector(const std::string &strSource, std::map<std::string, std::string> &map, const char* split, const char* pairSplit );
	void				split( const string& str, vector< string > & ret, const string& delims = "\t\n ", unsigned int maxSplits = 0);
	
	// 向量函数
	void				getXYdisFromVec(float &x, float &y, Vec2 v);

	// 获取旋转角度
	float				getDegreeByRadian(float r1, float r2);

	// 碰撞检测
	bool		isContainRect(Rect colRect, Point towerPos, float attackRange, Rect rect);

	Vec2		GetFixedPosion(float x, float y);
	float		GetFixedX(float x);
	float		GetFixedY(float y);
	float		GetXRate();
	float		GetYRate();
	float		GetDesignWidth();
	float		GetDesignHeight();

};

CUtils& Utils( );