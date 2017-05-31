//TDDBC_Struct.h
//�������ݿ�ṹ


#pragma once

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//							���ݱ��ʽ����								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
// �����
#define DBC_ENEMY		( 101 )
struct _DBC_ENEMY
{
	int				nID;				// ����ID
	int				nType;				// ����0С�� 1BOSS
	int				nResID;				// ��ԴID ���� 0 plane�� ���� 1 boss��
	const char*		szShooter;			// ��Ļ��Ӧdata/shooters
	int				nHP;				// Ѫ��
	int				nScore;				// ����
	int				nDiamondNum;		// ������������ʯ��
	int				nItemID;			// �������
};

// BOSS��
#define DBC_BOSS		( 102 )
struct _DBC_BOSS
{
	int				nID;				// ����ID
	const char*		szFileName;			// Animation�ļ�����
	const char*		szAnimation;		// ��������
	const char*		szRect1;			// ��ײ��Χ
	const char*		szRect2;			// ��ײ��Χ
	const char*		szDeadEffect;		// ������Ч
	const char*		szDeadAnimation;	// ������������
};

// С�ֱ�
#define DBC_PLANE		( 103 )
struct _DBC_PLANE
{
	int				nID;				// ����ID
	const char*		szAnimation;		// ��������
	int				nRect1;				// ��ײ��Χ1
	int				nRect2;				// ��ײ��Χ2
	int				nRect3;				// ��ײ��Χ3
	int				nRect4;				// ��ײ��Χ4
	const char*		szDeadEffect;		// ������Ч
	const char*		szDeadAnimation;	// ������������
};

// ��ұ�
#define DBC_USER		( 104 )
struct _DBC_USER
{
	int				nID;				// ����ID
	int				nRect1;				// ��ײ��Χ1
	int				nRect2;				// ��ײ��Χ2
	int				nRect3;				// ��ײ��Χ3
	int				nRect4;				// ��ײ��Χ4
	const char*		szShooter;			// ��Ļ
};

// �ɾͱ�
#define DBC_ACHIEVE		(105)
struct _DBC_ACHIEVE
{
	int				nID;				// ����ID
	const char*		szName;				// ����
	const char*		szDescribe;			// ����
	int				nType;				// ��������
	int				nNum;				// ��������
};


// �ӵ���
#define  DBC_BULLET		(106)
struct _DBC_BULLET
{
	int				nID;				// ����ID
	int				nType;				// �ӵ�����0Animation 1particle
	const char*		szName;				// �ӵ�����
	int				nRect1;				// ��ײ��Χ1
	int				nRect2;				// ��ײ��Χ2
	int				nRect3;				// ��ײ��Χ3
	int				nRect4;				// ��ײ��Χ4
	int				nValue1;			// �ӵ��Ƿ���Ҫָ������
	int				nValue2;			// ����
	int				nValue3;			// �ӵ��˺��ӳ�
};

// ս��������
#define DBC_UPGRADE		(107)
struct _DBC_UPGRADE
{
	int				nID;				// ����ID
	int				nCost;				// ˮ������
	const char*		szDescribe;			// ����
};

// ������Ч��
#define DBC_SOUND		(108)
struct _DBC_SOUND
{
	int		nSoundID;				// ��ЧID
	const	char* szDesc;			// ����
	const	char* szPath;			// �ļ�·��
	int		nType;					// ����:0��������,1��Ч
	int		nLoop;					// �Ƿ�ѭ������
};

// �ؿ���Ϣ��
#define DBC_MISSION_INFO (109)
struct	_DBC_MISSION_INFO
{
	int				nMissionID;				// �ؿ�ID��ҲΪ������
	char*			szStarImage;			// ����ͼƬ
	char*			szLockStarNameImage;	// ������ͼƬ-δ����
	char*			szStarNameImage;		// ��������-�ѽ���
	int				nStarX;					// ����X����
	int				nStarY;					// ����Y����
	char*			szBossFile;				// boss�ļ�����
	char*			szBossXoffset;			// bossλ��xƫ��
	char*			szBossYoffset;			// bossλ��yƫ��
	char*			szBossScale;			// ����ֵ
	char*			szBossNameImage;		// boss����ͼƬ
	char*			szBossInfoIamge;		// boss����ͼƬ
	int				nBossHp;				// Ѫ��
	int				nBossAttack;			// ����
	int				nBossSpeed;				// �ٶ�

	int				GetMissionID(){return nMissionID;}							// �ؿ�ID��ҲΪ������
	char*			GetStarImage(){ return szStarImage; }						// ����ͼƬ
	char*			GetLockStarNameImage(){return szLockStarNameImage;}			// ������ͼƬ-δ����
	char*			GetStarNameImage(){ return szStarNameImage; }				// ��������-�ѽ���
	int				GetStarPosX(){return nStarX;}								// ����X����
	int				GetStarPosY(){return nStarY;}								// ����Y����
	char*			GetBossFileName(){ return szBossFile; }						// boss�ļ�����
	char*			GetBossOffsetX(){ return szBossXoffset; }					// bossλ��xƫ��
	char*			GetBossOffsetY(){ return szBossYoffset; }					// bossλ��yƫ��
	char*			GetBossScale(){ return szBossScale; }						// ����ֵ
	char*			GetBossNameImage(){ return szBossNameImage; }				// boss����ͼƬ
	char*			GetBossInfoImage(){return szBossInfoIamge;}					// boss����ͼƬ
	int				GetBossHp(){ return nBossHp; }								// Ѫ��
	int				GetBossAttack(){return nBossAttack;}						// ����
	int				GetBossSpeed(){ return nBossSpeed; }						// �ٶ�

};


/*---------------------------------------------------------------------*/
// ���ò�����
#define DBC_PARAM_INFO (205)
struct	_DBC_PARAM_INFO
{
	static const int	INDEX_ID_COLUMN = 0;

	int				nTypeID;				// ID
	int				nIntValue;				// int��ֵ
	const char*		szDesc;					// �ַ���ֵ

	int				getID() const { return nTypeID;}
	int				getIntValue() const { return nIntValue;}
	const char*		getStringValue() const { return szDesc;}
};

// �Ʒ�
#define DBC_CHARGING_INFO (206)
struct	_DBC_CHARGING_INFO
{
	int				nTypeID;				// ID ͬ BuyType ��Ӧ
	char*			szName;					// ����
	int				nIntValue;				// Ǯ

	int				getID() const { return nTypeID;}
	int				getIntValue() const { return nIntValue;}
};

//������
#define DBC_TALK_FILTER	(207)
struct _DBC_TALK_FILTER
{
	int				nID;			// ID
	const char* 	szString;		// ���˴ʻ�(��������˵�Ĵʻ�)

	int getID() const {return nID;}
	const char* getString() const {return szString;}
};