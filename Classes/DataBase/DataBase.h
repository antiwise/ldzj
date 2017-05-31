/****************************************\
*										*
* 			  数据库系统				*
*										*
\****************************************/

#pragma once
#include "DataBase/LXDBC_Struct.h"
#include "DBC.h"
#include <map>
#include <assert.h>

//----------------------------------------------------------
//数据库
class CDataBase
{
public:
	CDataBase(int id);
	virtual ~CDataBase();

	//字段数据类型
	enum DBC_FIELD_TYPE
	{
		T_INT		= 0,	//整数
		T_FLOAT		= 1,	//浮点数
		T_STRING	= 2,	//字符串
	};

	//数据段
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

	//按照索引查找
	virtual const DBC_FIELD*	Search_Index_EQU(int iIndexValue) const;
	//按照行
	virtual const DBC_FIELD*	Search_LineNum_EQU(int iLineNum) const;
	//查找某列等于指定值的第一行
	virtual const DBC_FIELD*	Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& val) const;
	

public:
	virtual	const DBC::DBCFile*	GetDBCFile( void )const { return m_pFileDBC; }
	//取得ID
	virtual int					GetID(void)const;
	//取得列数
	virtual unsigned int		GetFieldsNum(void)const;
	//取得记录的条数
	virtual unsigned int		GetRecordsNum(void)const;
	//直接打开一个dbc文件
	static bool					Util_OpenDBCFromTxt(const char * szFileName, DBC::DBCFile* pDBCFile);
	
protected:
	//打开文本文件，生成一个数据库
	bool					OpenFromTXT(const char * szFileName);
	//保存一个dbc文件
	bool					SaveToTXT(int fileID, const char * szFileName);
	
protected:
	DBC::DBCFile*			m_pFileDBC;

	friend class CDataBaseSystem;
};

//----------------------------------------------------------
//数据库管理器
class CDataBaseSystem
{
public:
	//打开系统中所有数据库
	virtual void				OpenAllDataBase(void);
	//关闭系统中所有数据库
	virtual void				CloseAllDataBase(void);
	//得到一个已近打开的数据库，返回数据库指针
	virtual const CDataBase*	GetDataBase(int idDataBase) const;
	//得到角色数据
	virtual const CDataBase*	GetPlayerData(int idDataBase) const;


public:
	virtual void				Initial(void*);
	virtual void				Release(void);
	 
protected:
	//数据库
	std::map< int, CDataBase* >	m_mapDataBaseBuf;
	//角色数据
	std::map< int, CDataBase* >	m_mapPlayerDataBuf;

public:
	CDataBaseSystem();
	virtual ~CDataBaseSystem();
};

CDataBaseSystem &DataBaseSystem();

//---------------------------------------------------
//定义数据库快捷方式
#define DBC_DEFINEHANDLE(nameDBC, idDBC) \
	const static CDataBase* nameDBC = nullptr; \
	if (!nameDBC) {nameDBC = DataBaseSystem().GetDataBase(idDBC);} \
	assert(nameDBC);
