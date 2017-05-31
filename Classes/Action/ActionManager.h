#pragma once
#include <map>
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class LXAction;
//����Action�� ͨ�������ı��������Ŷ���
class CActionManager
{
public:
	CActionManager(void);
	~CActionManager(void);

public:
	
	// ���ض����ļ�
	void				LoadAction(const std::string &fileName);

	// ж�ض����ļ�
	void				UnLoadAction(const std::string &fileName);

	// ִ�ж���
	void				DoAction(Node* pNode, const std::string &actionName, int tag);

	//add by max
	ActionInterval*				getAct(const std::string &actionName, int tag);

private:

	// ��ȡJson�ļ�
	bool				readJson(const std::string &fileName, rapidjson::Document &doc);

	// ���������ļ�
	void				parseActionFile(const std::string &filename, rapidjson::Document &doc);

private:
	std::map<std::string, LXAction*> m_ActionMap;

};

CActionManager &LXActionManager();
