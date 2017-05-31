#include "ActionManager.h"
#include "Common/FileUtils.h"
#include "Action.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocostudio;

CActionManager& LXActionManager( )
{
	static CActionManager s;
	return s;
}

CActionManager::CActionManager(void)
{
}

CActionManager::~CActionManager(void)
{
	for (std::map<std::string, LXAction*>::iterator it = m_ActionMap.begin(); it != m_ActionMap.end(); it++)
	{
		// 删除信息
		delete it->second;
	}

	m_ActionMap.clear();
}

void CActionManager::LoadAction(const std::string &fileName)
{
    rapidjson::Document jsonDict;
    do {
		CC_BREAK_IF(!readJson(fileName, jsonDict));
        
		// 解析动画文件
		parseActionFile(fileName, jsonDict);
    } while (0);
}

bool CActionManager::readJson(const std::string &fileName, rapidjson::Document &doc)
{
    bool bRet = false;
    do {
		std::string fullPath = fileName + ".json";
		LXFileUtils::GetActionFullPath(fullPath);
        std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
        doc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(doc.HasParseError());
        bRet = true;
    } while (0);
    return bRet;
}

void CActionManager::parseActionFile(const std::string &filename, rapidjson::Document &doc)
{
	const rapidjson::Value &eventsDict = DICTOOL->getSubDictionary_json(doc, "event");
	// 解析	
	int count = eventsDict.Size();
	for (int i = 0; i < count; i++)
    {
		// 解析内容
		const rapidjson::Value &eventDict = DICTOOL->getSubDictionary_json(eventsDict, i);
		if (!DICTOOL->checkObjectExist_json(eventDict))
        {
            break;
        }
		
		// 获取动画名称
		std::string id = DICTOOL->getStringValue_json(eventDict, "eventID");
		if (m_ActionMap.find(id) != m_ActionMap.end())
		{
			std::string tempStr = id + "重名了!";
			MessageBox( tempStr.c_str(), "Error!!!" );
			break;
		}

		LXAction *pAction = new LXAction();
		
		std::string name = DICTOOL->getStringValue_json(eventDict, "name");
		
		if(eventDict["info"].IsNull() == false && eventDict["info"].Size() > 0)
		{
			std::string info = DICTOOL->getStringValue_json(eventDict, "info");
			pAction->SetInfo(info);
		}

		const rapidjson::Value &scriptsDict = DICTOOL->getSubDictionary_json(eventDict, "script");
		int scriptCount = scriptsDict.Size();
		for (int j = 0; j < scriptCount; j++)
		{
			const rapidjson::Value &eventDict = DICTOOL->getSubDictionary_json(scriptsDict, j);
			pAction->AddScriptCommand(eventDict.GetString());
		}
		pAction->SetEventID(id);
		pAction->SetName(name);

		m_ActionMap[id] = pAction;
	}
}

void CActionManager::DoAction(Node* pNode, const std::string &actionName, int tag)
{
	// 执行动作
	if (pNode == nullptr)
	{
		return;
	}

	std::map<std::string, LXAction*>::iterator itFind = m_ActionMap.find(actionName);
	if (itFind == m_ActionMap.end())
	{
		return;
	}

	LXAction *pActionData = itFind->second;
	Action *pAction = pActionData->ParseScript(startIndex);
	pAction->setTag(tag);
	// 解析脚本
	pNode->runAction(pAction);
}

ActionInterval*	CActionManager::getAct(const std::string &actionName, int tag)
{
	std::map<std::string, LXAction*>::iterator itFind = m_ActionMap.find(actionName);
	if (itFind == m_ActionMap.end())
	{
		return nullptr;
	}

	LXAction *pActionData = itFind->second;
	ActionInterval *pAction = dynamic_cast<ActionInterval*>(pActionData->ParseScript(startIndex));
	return pAction;
}