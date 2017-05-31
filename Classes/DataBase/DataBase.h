/****************************************\
*										*
* 			  ���ݿ�ϵͳ				*
*										*
\****************************************/

#pragma once
#include "DataBase/LXDBC_Struct.h"
#include "DBC.h"
#include <map>
#include <assert.h>

//----------------------------------------------------------
//���ݿ�
class CDataBase
{
public:
	CDataBase(int id);
	virtual ~CDataBase();

	//�ֶ���������
	enum DBC_FIELD_TYPE
	{
		T_INT		= 0,	//����
		T_FLOAT		= 1,	//������
		T_STRING	= 2,	//�ַ���
	};

	//���ݶ�
	union DBC_FIELD
	{
		float		fValue;
		int			iValue;
		const char*	pString;	// Just for runtime!

		//Construct
		DBC_FIELD() {}
		DBC_FIELD(int value) { iValue = value; }
		DBC_FIELD(float value) { fValue = value; }
		DBC_FIELD(const char* value) { pString = value; }
	};

public:

	//������������
	virtual const DBC_FIELD*	Search_Index_EQU(int iIndexValue) const;
	//������
	virtual const DBC_FIELD*	Search_LineNum_EQU(int iLineNum) const;
	//����ĳ�е���ָ��ֵ�ĵ�һ��
	virtual const DBC_FIELD*	Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& val) const;
	

public:
	virtual	const DBC::DBCFile*	GetDBCFile( void )const { return m_pFileDBC; }
	//ȡ��ID
	virtual int					GetID(void)const;
	//ȡ������
	virtual unsigned int		GetFieldsNum(void)const;
	//ȡ�ü�¼������
	virtual unsigned int		GetRecordsNum(void)const;
	//ֱ�Ӵ�һ��dbc�ļ�
	static bool					Util_OpenDBCFromTxt(const char * szFileName, DBC::DBCFile* pDBCFile);
	
protected:
	//���ı��ļ�������һ�����ݿ�
	bool					OpenFromTXT(const char * szFileName);
	//����һ��dbc�ļ�
	bool					SaveToTXT(int fileID, const char * szFileName);
	
protected:
	DBC::DBCFile*			m_pFileDBC;

	friend class CDataBaseSystem;
};

//----------------------------------------------------------
//���ݿ������
class CDataBaseSystem
{
public:
	//��ϵͳ���������ݿ�
	virtual void				OpenAllDataBase(void);
	//�ر�ϵͳ���������ݿ�
	virtual void				CloseAllDataBase(void);
	//�õ�һ���ѽ��򿪵����ݿ⣬�������ݿ�ָ��
	virtual const CDataBase*	GetDataBase(int idDataBase) const;
	//�õ���ɫ����
	virtual const CDataBase*	GetPlayerData(int idDataBase) const;


public:
	virtual void				Initial(void*);
	virtual void				Release(void);
	 
protected:
	//���ݿ�
	std::map< int, CDataBase* >	m_mapDataBaseBuf;
	//��ɫ����
	std::map< int, CDataBase* >	m_mapPlayerDataBuf;

public:
	CDataBaseSystem();
	virtual ~CDataBaseSystem();
};

CDataBaseSystem &DataBaseSystem();

//---------------------------------------------------
//�������ݿ��ݷ�ʽ
#define DBC_DEFINEHANDLE(nameDBC, idDBC) \
	const static CDataBase* nameDBC = nullptr; \
	if (!nameDBC) {nameDBC = DataBaseSystem().GetDataBase(idDBC);} \
	assert(nameDBC);
