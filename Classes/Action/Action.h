#pragma once
#include <vector>
#include "cocos2d.h"

const int startIndex = 0x80000000;

USING_NS_CC;
//�Զ���Ļ��ͨ�������ű���ʵ�ֶ���
class LXAction
{
public:
	LXAction();
	~LXAction();
	
public:

	void				SetEventID(const std::string &value) { m_EventID = value; }
	std::string&		GetEventID() { return m_EventID; }
	
	void				SetName(const std::string &value) { m_Name = value; }
	std::string&		GetName() { return m_Name; }
	 
	void				SetInfo(const std::string &value) { m_Info = value; }
	std::string&		GetInfo() { return m_Info; }
	
	void				AddScriptCommand(const std::string &name);

	int					GetXPos(int pos);
	int					GetYPos(int pos);

public:
	FiniteTimeAction*	ParseScript(int loop);
	
	FiniteTimeAction*	CreateAction(std::vector<std::string> &CommandVec);

private:
	std::string m_EventID;		// �¼�ID
	std::string m_Name;			// �¼�����
	std::string m_Info;			// ��Ϣ

	int			m_nIndex;						// ��ǰָ�����
	std::vector<std::string>	m_Commands;		// ָ��
};
