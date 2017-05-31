#include "AnimationManager.h"
#include "Element.h"
#include "Frame.h"
#include "Animation.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AnimationLogic.h"
#include "Common/FileUtils.h"

USING_NS_CC;
using namespace cocostudio;

CAnimationManager::CAnimationManager(void)
{
}

CAnimationManager::~CAnimationManager(void)
{
	for (std::map<std::string, LXAnimation*>::iterator it = m_AnimationMap.begin(); it != m_AnimationMap.end(); it++)
	{
		// 删除信息
		delete it->second;
	}

	m_AnimationMap.clear();
}

CAnimationManager& AnimationManager( )
{
	static CAnimationManager s;
	return s;
}

bool CAnimationManager::isAnimationFileLoaded(std::string &fileName)
{
	std::list<std::string>::iterator it = m_AnimationFiles.begin();
	while (it != m_AnimationFiles.end())
	{
		if (*it == fileName)
		{
			return true;
		}

		it++;
	}

	return false;
}

void CAnimationManager::LoadAnimation(std::string &fileName)
{
	if (isAnimationFileLoaded(fileName))
	{
		return;
	}

    rapidjson::Document jsonDict;
    do {
		CC_BREAK_IF(!readJson(fileName, jsonDict));
        
		// 解析动画文件
		parseAnimationFile(fileName, jsonDict);
		m_AnimationFiles.push_back(fileName);

    } while (0);
}

bool CAnimationManager::readJson(const std::string &fileName, rapidjson::Document &doc)
{
    bool bRet = false;
    do {
		std::string fullPath = fileName + ".json";
		LXFileUtils::GetAnimationFullPath(fullPath);
        std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
        doc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(doc.HasParseError());
        bRet = true;
    } while (0);
    return bRet;
}

std::string CAnimationManager::GetKeyName(std::string &lh, const std::string &rh)
{
	return lh + "@" + rh;
}

void CAnimationManager::parseAnimationFile(std::string &filename, rapidjson::Document &doc)
{
	// 解析	
	int count = doc.Size();
	for (int i = 0; i < count; i++)
    {
		// 解析内容
		const rapidjson::Value &subDict = DICTOOL->getSubDictionary_json(doc, i);
		if (!DICTOOL->checkObjectExist_json(subDict))
        {
            break;
        }
		
		// 获取动画名称
		std::string name = DICTOOL->getStringValue_json(subDict, "name");
		std::string key = GetKeyName(filename, name);
		if (m_AnimationMap.find(key) != m_AnimationMap.end())
		{
			break;
		}
		LXAnimation *pAnimation = new LXAnimation();

		float originX = DICTOOL->getFloatValue_json(subDict, "originX");
		float originY = DICTOOL->getFloatValue_json(subDict, "originY");
		float w = DICTOOL->getFloatValue_json(subDict, "w");
		float h = DICTOOL->getFloatValue_json(subDict, "h");
		
		if (subDict.HasMember("script") && subDict["script"].IsNull() == false && subDict["script"].Size() > 0)
		{
			std::string script = DICTOOL->getStringValue_json(subDict, "script");
			pAnimation->SetScript(script);
		}

		pAnimation->SetPath(filename);
		pAnimation->SetName(name);
		pAnimation->SetOriginX(originX);
		pAnimation->SetOriginY(originY);
		pAnimation->SetWidth(w);
		pAnimation->SetHeight(h);

		m_AnimationMap[key] = pAnimation;

		const rapidjson::Value &framesDict = DICTOOL->getSubDictionary_json(subDict, "frames");
		int frameCount = framesDict.Size();

		for (int j = 0; j < frameCount; j++)
		{
			// 解析内容
			const rapidjson::Value &subFrameDict = DICTOOL->getSubDictionary_json(framesDict, j);
			if (!DICTOOL->checkObjectExist_json(subFrameDict))
			{
				break;
			}

			// 创建关键帧
			LXFrame *pFrame = new LXFrame();
			pAnimation->AddFrame(pFrame);
			// 读取关键帧信息
			// 获取动画名称
			int delay = DICTOOL->getIntValue_json(subFrameDict, "delay");
			if(subFrameDict["script"].IsNull() == false && subFrameDict["script"].Size() > 0)
			{
				std::string script = DICTOOL->getStringValue_json(subFrameDict, "script");
				pFrame->SetScript(script);
			}
			pFrame->SetDelay(delay);

			// 读取元素信息
			const rapidjson::Value &elementsDict = DICTOOL->getSubDictionary_json(subFrameDict, "elements");
			int elementCount = elementsDict.Size();
			for (int k = 0; k < elementCount; k++)
			{
				// 解析内容
				const rapidjson::Value &subElementDict = DICTOOL->getSubDictionary_json(elementsDict, k);
				if (!DICTOOL->checkObjectExist_json(subElementDict))
				{
					break;
				}

				// 解析元素信息
				LXElement *pElement = new LXElement();
				pFrame->AddElement(pElement);
				
				std::string res_dir = DICTOOL->getStringValue_json(subElementDict, "res_dir");
				std::string res_name = DICTOOL->getStringValue_json(subElementDict, "res_name");
				int res_type = DICTOOL->getIntValue_json(subElementDict, "res_type");
				if (res_type == 2)
				{
					res_dir = filename;
				}
				
				std::string name = DICTOOL->getStringValue_json(subElementDict, "name");
				int curFrame	= DICTOOL->getIntValue_json(subElementDict, "curFrame");
				float x			= DICTOOL->getFloatValue_json(subElementDict, "x");
                float y			= DICTOOL->getFloatValue_json(subElementDict, "y");
                float w			= DICTOOL->getFloatValue_json(subElementDict, "w");
                float h			= DICTOOL->getFloatValue_json(subElementDict, "h");
                float originX	= DICTOOL->getFloatValue_json(subElementDict, "originX");
                float originY	= DICTOOL->getFloatValue_json(subElementDict, "originY");
                float scaleX	= DICTOOL->getFloatValue_json(subElementDict, "scaleX");
                float scaleY	= DICTOOL->getFloatValue_json(subElementDict, "scaleY");
                float rotation	= DICTOOL->getFloatValue_json(subElementDict, "rotation");
				int transMode	= DICTOOL->getIntValue_json(subElementDict, "transMode");
				int argb	= DICTOOL->getIntValue_json(subElementDict, "argb");
				
				if(subElementDict["script"].IsNull() == false && subElementDict["script"].Size() > 0)
				{
					std::string script = DICTOOL->getStringValue_json(subElementDict, "script");
					pElement->SetScript(script);
				}
					
				if (transMode != 0)
				{
					if ((1 & transMode) != 0)
					{
						//rotation = 360.0f - rotation;
						originX = w - originX;
					}
					
					if ((2 & transMode) != 0)
					{
						//rotation = 360.0f - rotation;
						originY = h - originY;
					}
				}
				
				pElement->SetResDir(res_dir);
				pElement->SetResName(res_name);
				pElement->SetResType(res_type);
				pElement->SetName(name);					
				pElement->SetCurFrame(curFrame);			
				pElement->SetX(x);							
				pElement->SetY(y);							
				pElement->SetWidth(w);						
				pElement->SetHeight(h);						
				pElement->SetOriginX(originX);					
				pElement->SetOriginY(originY);					
				pElement->SetScaleX(scaleX);						
				pElement->SetScaleY(scaleY);						
				pElement->SetRotation(rotation);			
				pElement->SetTransMode(transMode);			
				pElement->SetArgb(argb);
			}
		}
	}
}

LXAnimationLogic* CAnimationManager::CreateAnimationNode(CCNode *pNode, std::string &name, const std::string &key, std::string &nameFix, bool flip)
{
	// 判断动画文件是否已经解析
	if (isAnimationFileLoaded(name) == false)
	{
		LoadAnimation(name);
	}

	// 根据动画信息创建动画控制逻辑 
	std::string animKey = AnimationManager().GetKeyName(name, key);
	LXAnimationLogic *p	= new LXAnimationLogic(pNode, GetAnimation(animKey), nameFix, flip);

	return p;
}

// 获取动画信息
LXAnimation* CAnimationManager::GetAnimation(std::string &name)
{
	// 获取完整动画名称
	std::map<std::string, LXAnimation*>::iterator itFind = m_AnimationMap.find(name);
	if (itFind == m_AnimationMap.end())
	{
		//CCLog("没有找到动画文件: %s", name.c_str());
		return NULL;
	}

	//LXAnimation *pAnimation = new LXAnimation(*itFind->second);
	return itFind->second;
}