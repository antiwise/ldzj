#include "DataBase.h"
#include "cocos2d.h"
#include "DBC.h"
#include "GameDefine.h"

USING_NS_CC;
//============================================================================

struct _DATABASE_DEFINE
{
	int			idDatabase;
	const char*		pFileName;
};

_DATABASE_DEFINE  s_dbToLoad[] = 
{
	{ DBC_ENEMY,			"db/enemy.txt"			},				// �����
	{ DBC_BOSS,				"db/boss.txt"			},				// BOSS��
	{ DBC_PLANE,			"db/plane.txt"			},				// С�ֱ�
	{ DBC_USER,				"db/user.txt"			},				// ��ұ�
	{ DBC_ACHIEVE,			"db/achieve.txt"		},				// �ɾͱ�
	{ DBC_BULLET,			"db/bullet.txt"			},				// �ӵ���
	{ DBC_UPGRADE,			"db/upgrade.txt"		},				// ǿ����
	{ DBC_PARAM_INFO,		"db/Parameter.txt"		},				// ������
	{ DBC_CHARGING_INFO,	"db/charging.txt"		},				// �Ʒѱ�
	{ DBC_MISSION_INFO,		"db/MissionInfo.txt"	},				// �ؿ�ѡ����Ϣ��
	{ DBC_TALK_FILTER,		"db/StrFilter.txt"		},				// ������

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{ DBC_SOUND,			"db/sound.txt"			},				// ��Ч��
//#else 
//	{ DBC_SOUND,			"db/sound_win32.txt"	},				// ��Ч��
//#endif

};

CDataBase::CDataBase(int id)
{
	m_pFileDBC = new DBC::DBCFile(id);
}

CDataBase::~CDataBase()
{
	delete m_pFileDBC;
}

const CDataBase::DBC_FIELD* CDataBase::Search_Index_EQU(int iIndexValue) const 
{
	return (DBC_FIELD*)m_pFileDBC->Search_Index_EQU(iIndexValue);
}

const CDataBase::DBC_FIELD* CDataBase::Search_LineNum_EQU(int iLineNum) const
{
	return  (DBC_FIELD*)m_pFileDBC->Search_Posistion(iLineNum, 0); 
}

//����ĳ�е���ָ��ֵ�ĵ�һ��
const CDataBase::DBC_FIELD* CDataBase::Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& val) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_First_Column_Equ(nColumnNum, val.iValue);
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

//ȡ��ID
int CDataBase::GetID(void) const
{
	return m_pFileDBC->GetID(); 
}

//ȡ������
unsigned int CDataBase::GetFieldsNum(void)const
{ 
	return m_pFileDBC->GetFieldsNum(); 
}

//ȡ�ü�¼������
unsigned int CDataBase::GetRecordsNum(void)const
{ 
	return m_pFileDBC->GetRecordsNum(); 
}

//ֱ�Ӵ�һ��dbc�ļ�
bool CDataBase::Util_OpenDBCFromTxt(const char *szFileName, DBC::DBCFile* pDBCFile)
{
	assert(szFileName && pDBCFile);

	char* lpAddress = 0;
    unsigned char* pBuffer = 0;
    ssize_t bufferSize = 0;
    pBuffer = FileUtils::getInstance()->getFileData(szFileName, "rb", &bufferSize);
	const char* pCBuffer = const_cast<const char*>(reinterpret_cast<char*>(pBuffer));
	if( bufferSize > 0 )
	{
		bool bRet = pDBCFile->OpenFromMemory( pCBuffer, pCBuffer + bufferSize);
		delete [] pBuffer;
		pBuffer = 0;
		return bRet;
	}
	return false;
}

bool CDataBase::OpenFromTXT(const char *szFileName)
{
	return Util_OpenDBCFromTxt(szFileName, m_pFileDBC);
}

bool CDataBase::SaveToTXT(int fileID, const char * szFileName)
{
	assert(szFileName && m_pFileDBC);

	char* lpAddress = 0;
	unsigned char* pBuffer = 0;
	ssize_t bufferSize = 0;
	pBuffer = FileUtils::getInstance()->getFileData(szFileName, "rb", &bufferSize);
	if( bufferSize > 0 )
	{
		FILE *fp = fopen(szFileName, "w+");
		char* pTargetBuffer = new char[1024*1024];
		memset(pTargetBuffer, 0, 1024*1024);
		unsigned long targetSize = 0;
		bool bRet = m_pFileDBC->FillData(fileID, (const char*)pBuffer, bufferSize, pTargetBuffer, &targetSize);
		if(bRet)
		{
			fwrite(pTargetBuffer, sizeof(char), targetSize, fp);
		}
		fclose(fp);

		delete [] pBuffer;
		delete [] pTargetBuffer;
		pBuffer = 0;
		pTargetBuffer = 0;
		return bRet;
	}

	return false;
}


CDataBaseSystem &DataBaseSystem()
{
	static CDataBaseSystem a;
	return a;
}

CDataBaseSystem::CDataBaseSystem()
{
}

CDataBaseSystem::~CDataBaseSystem()
{
}

void CDataBaseSystem::Initial(void*)
{
	OpenAllDataBase();
}

void CDataBaseSystem::Release(void)
{
	CloseAllDataBase();
}

void CDataBaseSystem::OpenAllDataBase(void)
{
	//���������ݿ�
	int nNum = sizeof(s_dbToLoad)/sizeof(_DATABASE_DEFINE);
	for(int i=0; i<nNum; i++)
	{
		CDataBase* pDataBase = new CDataBase(s_dbToLoad[i].idDatabase);
	
		std::string strFileName = s_dbToLoad[i].pFileName;

		if(!(pDataBase->OpenFromTXT (strFileName.c_str())))
		{
			char szLogInfo[256]={0};
			sprintf(szLogInfo,"Open %s Error!",strFileName.c_str());
			//CCLog(szLogInfo);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			MessageBox(szLogInfo,"tishi");
#endif
		}
		if(m_mapDataBaseBuf.find(s_dbToLoad[i].idDatabase) != m_mapDataBaseBuf.end())
		{
			char szLogInfo[256]={0};
			sprintf(szLogInfo,"DBC ID Multi define(%s)",strFileName.c_str());
			//CCLog(szLogInfo);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			MessageBox(szLogInfo,"tishi");
#endif
		}
		m_mapDataBaseBuf.insert(std::make_pair(s_dbToLoad[i].idDatabase, pDataBase));
	}

}

void CDataBaseSystem::CloseAllDataBase(void)
{
	std::map< int, CDataBase* >::iterator it;
	for(it = m_mapDataBaseBuf.begin(); it!=m_mapDataBaseBuf.end(); it++)
	{
		delete (CDataBase*)(it->second);
		it->second = 0;
	}
 
	for(it = m_mapPlayerDataBuf.begin(); it!=m_mapPlayerDataBuf.end(); it++)
	{
		delete (CDataBase*)(it->second);
		it->second = 0;
	}

	m_mapDataBaseBuf.clear();
	m_mapPlayerDataBuf.clear();
}

const CDataBase* CDataBaseSystem::GetDataBase(int idDataBase) const
{
 	std::map< int, CDataBase* >::const_iterator it = m_mapDataBaseBuf.find(idDataBase);
 	return (const CDataBase*)it->second;
}

//�õ���ɫ����
const CDataBase* CDataBaseSystem::GetPlayerData(int idDataBase) const
{
 	std::map< int, CDataBase* >::const_iterator it = m_mapPlayerDataBuf.find(idDataBase);
 	return (const CDataBase*)it->second;
}

