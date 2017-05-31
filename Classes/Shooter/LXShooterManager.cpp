#include "LXShooterManager.h"
#include "LXShooter.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"

LXShooterManager &ShooterManager()
{
	static LXShooterManager a;
	return a;
}

LXShooterManager::LXShooterManager()
{
}

LXShooterManager::~LXShooterManager() 
{
}

void LXShooterManager::ParseShootersData()
{
	//int i = GetTickCount();
	// ������ҵ�Ļ�ļ�
	std::string userPath = "user/list.txt";
	LXFileUtils::GetShooterFullPath(userPath);
	parseShooterFile("user/", userPath);
	//log("time1 %d", GetTickCount() - i);

	// ���ع��ﵯĻ�ļ�
	std::string enemyPath = "list.txt";
	LXFileUtils::GetShooterFullPath(enemyPath);
	parseShooterFile("", enemyPath);
	//log("time2 %d", GetTickCount() - i);
}

void LXShooterManager::parseShooterFile(std::string path, std::string& filename)
{
	// ������Ļ���ļ�
	//�ļ��Ƿ����
	std::istringstream ifs(FileUtils::getInstance()->getStringFromFile(filename));
	if (!ifs)
	{
		log("Cannot open file [%s]",filename.c_str());
		return;
	}

	int maxchars = 8192;				// Alloc enough size.
	std::vector<char> buf(maxchars);	// Alloc enough size.
	while (ifs.peek() != -1)
	{
		ifs.getline(&buf[0], maxchars);
		std::string linebuf(&buf[0]);


		// Trim newline '\r\n' or '\r'
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\n') linebuf.erase(linebuf.size()-1);
		}
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\r') linebuf.erase(linebuf.size()-1);
		}

		// Skip if empty line.
		if (linebuf.empty())
		{
			continue;
		}
	
		parseShooterData(path, linebuf);
	}
}

void LXShooterManager::parseShooterData(std::string& path, std::string& filename)
{
	// ������Ļ���ļ�1
	//�ļ��Ƿ����
	std::string targetPath = path;
	LXFileUtils::GetShooterFullPath(targetPath);
	std::string shooterFilePath = targetPath + filename;
	std::istringstream ifs(FileUtils::getInstance()->getStringFromFile(shooterFilePath));
	if (!ifs)
	{
		log("Cannot open file [%s]",filename.c_str());
		return;
	}

	LXShooterGroup *pShooterGroup = new LXShooterGroup();
	m_LXShootersMap[filename.substr(0, filename.length() -3)] = pShooterGroup;
	bool elementStart = false;			// ��ʼ������Ԫ��
	LXShooter *pShooter = nullptr;		// Ԫ��
	int maxchars = 8192;				// Alloc enough size.
	std::vector<char> buf(maxchars);	// Alloc enough size.
	while (ifs.peek() != -1)
	{
		ifs.getline(&buf[0], maxchars);
		std::string linebuf(&buf[0]);

		// Trim newline '\r\n' or '\r'
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\n') linebuf.erase(linebuf.size()-1);
		}
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\r') linebuf.erase(linebuf.size()-1);
		}

		// Skip if empty line.
		if (linebuf.empty())
		{
			continue;
		}
	
		// ����
		if (linebuf.at(0) == '\0')
		{
			continue;
		}
		
		if (elementStart)
		{
			// ������Ԫ��ֱ������Ԫ�ؽ�β
			if (linebuf == "- end -")
			{
				// ��������
				elementStart = false;
				pShooter = nullptr;
			}
			else if (linebuf.at(0) != '-' && pShooter != nullptr)
			{
				if(linebuf.find("=") != std::string::npos)
				{
					std::vector<std::string> valuePair;
					Utils().split(linebuf, valuePair, "=");
					if (valuePair[0] == "bulletType ")
					{
						pShooter->SetType(atoi(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "model ")
					{
						pShooter->SetResId(atoi(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "offX ")
					{
						pShooter->SetOffX(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "offY ")
					{
						pShooter->SetOffY(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "group ")
					{
						pShooter->SetGroup(atoi(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "num ")
					{
						pShooter->SetNum(atoi(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "speed ")
					{
						pShooter->SetSpeed(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "speedDelta ")
					{
						pShooter->SetSpeedDelta(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "degree ")
					{
						pShooter->SetDegree(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "degreeDelta ")
					{
						pShooter->SetDegreeDelta(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "groupDegreeDelta ")
					{
						pShooter->SetGroupDegreeDelta(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "waitTime ")
					{
						pShooter->SetWaitTime(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "groupWaitTime ")
					{
						pShooter->SetGroupWaitTime(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "delay ")
					{
						pShooter->SetDelay(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "duration ")
					{
						pShooter->SetDuration(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "delay ")
					{
						pShooter->SetDelay(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "maxDeltaDegree ")
					{
						pShooter->SetMaxDeltaDegree(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "minSpeed ")
					{
						pShooter->SetMinSpeed(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "maxSpeed ")
					{
						pShooter->SetMaxSpeed(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "deltaSpeed ")
					{
						pShooter->SetDeltaSpeed(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "deltaSpeedChange ")
					{
						bool bValue = atoi(valuePair[1].c_str()) == 0 ? false : true;
						pShooter->SetDeltaSpeedChange(bValue);
					}
					else if (valuePair[0] == "burstTime ")
					{
						pShooter->SetBurstTime(atof(valuePair[1].c_str()));
					}
					else if (valuePair[0] == "canThrough ")
					{
						bool bValue = atoi(valuePair[1].c_str()) == 0 ? false : true;
						pShooter->SetCanThrough(bValue);
					}
					else if (valuePair[0] == "shooterPath ")
					{
						pShooter->SetPath(valuePair[1]);
					}
					else
					{
						//log("unrigister value pair %s = ", linebuf.c_str());
					}
				}
			}
		}
		else
		{
			// ��Ԫ�ؿ�ʼ
			if (linebuf.at(0) == '[' && linebuf.at(linebuf.length() - 1) == ']')
			{
				// ������Ļ
				pShooter = new LXShooter();
				std::string name = linebuf.substr(1, linebuf.length() - 2);
				pShooter->SetName(name);
				pShooterGroup->AddShooter(pShooter);
				elementStart = true;
			}
			else
			{
				if(linebuf.find("=") != std::string::npos)
				{
					std::vector<std::string> valuePair;
					Utils().split(linebuf, valuePair, "=");
					if (valuePair[0] == "offX")
					{
						// û�õ�������
					}
					else if (valuePair[0] == "offY")
					{
						// û�õ�������
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
		}
	}
}

LXShooterGroup* LXShooterManager::GetShooterByName(std::string& name)
{
	std::map<std::string, LXShooterGroup*>::iterator itFind = m_LXShootersMap.find(name);
	if (itFind != m_LXShootersMap.end())
	{
		return itFind->second;
	}	

	return NULL;
}