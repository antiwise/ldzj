#pragma once
#include "cocos2d.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// ��Ļ������
// ����Ļ���ݵĽ��������

class LXShooter;
class LXShooterGroup;
class LXShooterManager
{
public:
	LXShooterManager();
	~LXShooterManager();

public:
	// �������еĵ�Ļ�ļ�
	void				ParseShootersData();		

	// ͨ���ļ�����ȡ��Ļ��Ϣ
	LXShooterGroup*		GetShooterByName(std::string& name);

private:
	// ������Ļ�б�
	void				parseShooterFile(std::string path, std::string& filename);	

	// ������Ļ�ļ�
	void				parseShooterData(std::string& path, std::string& filename);	

private:
	
	// ����������߼�����
	std::map<std::string, LXShooterGroup*>		m_LXShootersMap;

};
LXShooterManager &ShooterManager();