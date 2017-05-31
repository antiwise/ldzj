/*
|==========================================
|	DBC数据库文件类
|		(服务器/客户端通用)
|==========================================
|
|		--------------------
|		|  数据库文件格式  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  unsigned int		DBC File Identity， always 0XDDBBCC00
|		0X004	  unsigned int      Number of records in the file 
|		0X008     unsigned int      Number of 4-byte fields per record
|		0X010     unsigned int      String block size 
|   FieldType
|		0X014     unsigned int[FieldNum]   
|							  The type fo fields(0-int, 1-float, 2-string)
|   FieldBlock
|				  unsigned int[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  char[StringSize]
|							  StringBlock
|
*/
#ifndef _DBC_H_
#define _DBC_H_

#include <vector>
#ifdef _WINDOWS
#include <hash_map>
#else
#include <ext/hash_map>
#endif

using namespace std;

namespace DBC
{
	class DBCFile	
	{
	public:
		//文件头
		struct FILE_HEAD
		{
			unsigned int		m_Identify;				//标示	0XDDBBCC00
			int					m_nFieldsNum;			//列数
			int					m_nRecordsNum;			//行数
			int					m_nStringBlockSize;		//字符串区大小
		};

		//字段数据类型
		enum FIELD_TYPE
		{
			T_INT		= 0,	//整数
			T_FLOAT		= 1,	//浮点数
			T_STRING	= 2,	//字符串
		};

		//数据库格式描述
		typedef std::vector< FIELD_TYPE >	FILEDS_TYPE;

		//数据段
		union FIELD
		{
			float		fValue;
			int			iValue;
			unsigned short		wValue;
			char		cValue;
			const char*	pString;	// Just for runtime!

			//Construct
			FIELD() {}
			FIELD(int val) { iValue = val; }
			FIELD(float val) { fValue = val; }
			FIELD(const char* val) { pString = val; }
		};
		//数据区
		typedef std::vector< FIELD >		DATA_BUF;

	public:
		//打开文本文件，生成一个数据库
		bool					OpenFromTXT(const char* szFileName);
		//根据内存中的文件打开
		bool					OpenFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
	public:	
		//写数据操作
		bool					FillData(int fileID, const char* pCBuffer, unsigned long bufferSize, char* pBuffer, unsigned long* targetSize);

	protected:
		//读取文本格式内容
		bool					OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
		//读取二进制格式内容
		bool					OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);

	public:
		//按索引查找(第一列为索引)
		virtual const FIELD*	Search_Index_EQU(int val) const;
		//按照位置查找
		virtual const FIELD*	Search_Posistion(int nRecordLine, int nColumNum) const;
		//查找某列等于指定值的第一行
		virtual const FIELD*	Search_First_Column_Equ(int nColumnNum, const FIELD& val) const;

	public:
		//取得ID
		unsigned int GetID(void) const				{ return m_ID; }
		//取得列数
		int	GetFieldsNum(void) const	    { return m_nFieldsNum; }
		//取得记录的条数
		int GetRecordsNum(void) const		{ return m_nRecordsNum; }
		//生成索引列
		void CreateIndex(int nColum = 0, const char* szFileName=0);

	protected:
#ifdef __SGI_STL_PORT
		typedef std::hash_map< int, FIELD*>	FIELD_HASHMAP;
#else
	#ifdef _WINDOWS
		typedef stdext::hash_map< int, FIELD*>	FIELD_HASHMAP;
	#else
		typedef __gnu_cxx::hash_map< int,FIELD*>    FIELD_HASHMAP;
							   // hash_compare <int, less<int> > ,
							   //allocator< pair<const int, FIELD*> > >
							  
	#endif
		
#endif
		//数据库格式文件名
		unsigned int			m_ID;
		//数据库格式描述
		FILEDS_TYPE				m_theType;
		//行数
		int						m_nRecordsNum;
		//列数
		int						m_nFieldsNum;
		//数据区
		DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//字符串区
		char*					m_pStringBuf;
		//字符串区大小
		int						m_nStringBufSize;
		//索引表
		FIELD_HASHMAP			m_hashIndex;
		//索引列
		int						m_nIndexColum;

	public:

		static int			_ConvertStringToVector(const char* strStrINTgSource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);
		//从内存中字符串读取一行文本(按照换行符)
		static const char*	_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);
		//比较两个值是否相等
		template < FIELD_TYPE T>	
		static bool			_FieldEqu(const FIELD& a, const FIELD& b);

	public:
		DBCFile(unsigned int id);
		virtual ~DBCFile();
	};
}

#endif