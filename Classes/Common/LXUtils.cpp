#include "LXUtils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
#include <stdarg.h>
#include <sys/stat.h>
#include<unistd.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
#include "io.h"
#include "direct.h"
#endif

CUtils& Utils( )
{
	static CUtils Alliance;
	return Alliance;
}

int CUtils::SplitPairStringToVector(const std::string &strSource, std::vector<std::pair<int,int> > &vect, const char* split, const char* pairSplit )
{
	vect.clear();

	if (strSource.empty() || !strlen(split) || !strlen(pairSplit))
	{
		return 0;
	}

	if (strSource.find(pairSplit) == std::string::npos)
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight = strSource.find_first_of(split); 

	for (;nRight != strSource.length(); nLeft = nRight + strlen(split))
	{
		nRight = strSource.find(split, nLeft);
		if (nRight == std::string::npos)
		{
			nRight = strSource.length();
		}

		//获取子项
		std::string strItem = strSource.substr(nLeft, nRight - nLeft);
		if (!strItem.empty())
		{
			std::string::size_type postion = strItem.find_first_of(pairSplit);
			if (postion == std::string::npos)
			{
				continue;
			}

			//转换成数字
			std::string strFirst = strItem.substr(0, postion);
			std::string strSecond= strItem.substr(postion+1,strItem.length()-postion);

			int nfirst = atoi(strFirst.c_str());
			int nSecond= atoi(strSecond.c_str());

			vect.push_back(std::make_pair(nfirst,nSecond));
		}

	}

	return vect.size();
}

int CUtils::SplitPairStringToVector(const std::string &strSource, std::map<std::string, std::string> &map, const char* split, const char* pairSplit )
{
	map.clear();

	if (strSource.empty() || !strlen(split) || !strlen(pairSplit))
	{
		return 0;
	}

	if (strSource.find(pairSplit) == std::string::npos)
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight = strSource.find_first_of(split); 

	for (;nRight != strSource.length(); nLeft = nRight + strlen(split))
	{
		nRight = strSource.find(split, nLeft);
		if (nRight == std::string::npos)
		{
			nRight = strSource.length();
		}

		//获取子项
		std::string strItem = strSource.substr(nLeft, nRight - nLeft);
		if (!strItem.empty())
		{
			std::string::size_type postion = strItem.find_first_of(pairSplit);
			if (postion == std::string::npos)
			{
				continue;
			}

			//转换成数字
			std::string strFirst = strItem.substr(0, postion);
			std::string strSecond= strItem.substr(postion+1,strItem.length()-postion);

			map[strFirst]=strSecond;
		}

	}

	return map.size();
}

void CUtils::split( const string& str, vector< string > & ret, const string& delims, unsigned int maxSplits)
{
    unsigned int numSplits = 0;

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            start = pos + 1;
        }
        else if (pos == string::npos || (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the string
            ret.push_back( str.substr(start) );
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back( str.substr(start, pos - start) );
            start = pos + 1;
        }
        // parse up to next real data
        start = str.find_first_not_of(delims, start);
        ++numSplits;

    } while (pos != string::npos);
}

// 向量函数
void CUtils::getXYdisFromVec(float &x, float &y, Vec2 v)
{
	// 标准化向量
	Vec2 normalVec = ccpNormalize(v);

	// 投影
	Vec2 px = ccpProject(normalVec, Vec2(1,0));  // X轴投影
	Vec2 py = ccpProject(normalVec, Vec2(0,1));  // Y轴投影

	x = ccpLength(px);
	y = ccpLength(py);
}

// 获取旋转角度
float CUtils::getDegreeByRadian(float r1, float r2)
{
	/*if (r2 - r1 > )
	{

	}*/
	
	return 0;
}

bool CUtils::isContainRect(Rect colRect, Point towerPos, float attackRange, Rect rect)
{
	bool  contain = false;
	Point orign = towerPos;
	
	Point rectVertex0 = rect.origin;											// 左下
	Point rectVertex1 = rect.origin + Point(rect.size.width, 0);				// 右下
	Point rectVertex2 = rect.origin + Point(rect.size.width, rect.size.height);	// 右下
	Point rectVertex3 = rect.origin + Point(0, rect.size.height);;				// 左上
	Point pos[] = {rectVertex0, rectVertex1, rectVertex2, rectVertex3};

	do{
		if((pos[1].x - orign.x) * (pos[3].x - orign.x) > 0 && 
			(pos[1].y - orign.y) * (pos[3].y - orign.y) > 0 )
		{
			// 右下顶点和左上顶点在同一象限，说明整个矩形区域位于一个象限之内
			for(int i = 0; i < 4; i++)
			{
				if((pos[i].x - orign.x) * (pos[i].x - orign.x) + (pos[i].y - orign.y) * (pos[i].y - orign.y) 
					< attackRange * attackRange)
				{
					contain = true;
					break;
				}
			}
		}

		if(colRect.intersectsRect(rect))
		{
			contain = true;
			break;
		}

		for(int i = 0; i < 4; i++)
		{
			if(pos[i] == orign)
			{
				contain = true;
				break;
			}
		}
	} while(false);

	return contain;
}


Vec2	CUtils::GetFixedPosion(float x, float y)
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const float xRate = screenSize.width/480;
	const float yRate = screenSize.height/800;

	return Vec2(x * xRate, y * yRate);
}

float	CUtils::GetFixedX(float x)
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const float xRate = screenSize.width/480;
	
	return x * xRate;
}

float	CUtils::GetFixedY(float y)
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const float yRate = screenSize.height/800;

	return y * yRate;
}

float	CUtils::GetXRate()
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	return screenSize.width/480;
}

float	CUtils::GetYRate()
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	return screenSize.height/800;
}

float	CUtils::GetDesignWidth()
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	return screenSize.width;
}
float	CUtils::GetDesignHeight()
{
	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	return screenSize.height;
}

void CUtils::copyDataFile(const char* pFileName, const char* pTargetPath)
{
	std::string strPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
	ssize_t len = 0;
	unsigned char *data = 0;

	CCLOG(strPath.c_str());
	data = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "r", &len);
	CCLOG("file:%s, len%d", pTargetPath, len);
	FILE* fp = fopen(pTargetPath, "w+");
	if(!fp)
	{
		CCLOG("file not found!");
	}
	fwrite(data, sizeof(char), len, fp);
	fclose(fp);
	delete []data;
	data = 0;
	
}

void CUtils::createDir(const char* szPath)
{
	std::string pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
	pathToSave += szPath;

	// Create the folder if it doesn't exist
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(access(pathToSave.c_str(), 0) != 0)
	{
		mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
#else
	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(pathToSave.c_str(), 0);
	}
#endif
}
