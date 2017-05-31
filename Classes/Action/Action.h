#pragma once
#include <vector>
#include "cocos2d.h"

const int startIndex = 0x80000000;

USING_NS_CC;
//自定义的活动，通过解析脚本来实现动画
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
	std::string m_EventID;		// 事件ID
	std::string m_Name;			// 事件名称
	std::string m_Info;			// 信息

	int			m_nIndex;						// 当前指令次序
	std::vector<std::string>	m_Commands;		// 指令
};
