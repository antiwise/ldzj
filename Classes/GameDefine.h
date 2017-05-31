/****************************************\
*										*
* 			   ��������					*
*										*
\****************************************/

#pragma once

const int ai_tag_common = 9999;
const int ai_tag_bossai = 9998;

#define TASK_COUNT 38

//����ö��
enum INTERFACE_ID
{
	INTERFACE_LOADING,		// ���ؽ���
	INTERFACE_MAIN,			// ������
	INTERFACE_INTENSIFY,	// ǿ������
	INTERFACE_MISSION,		// �ؿ�ѡ�����

};

//��Ϸ�����ڲ����
enum GAMESCENE_ZORDER
{
	GAME_ZORDER_MAP		= 0,		// ��ͼ
	GAME_ZORDER_ENEMY,				// �л�
	GAME_ZORDER_PLAYER_BULLET,		// ����ӵ�
	GAME_ZORDER_BULLET_EFFECT,		// ����ӵ���Ч
	GAME_ZORDER_PLANE,				// �ɻ�
	GAME_ZORDER_AI_BULLET,			// AI�ӵ�
	GAME_ZORDER_ITEM,				// ���ߵ���
	GAME_ZORDER_OBJECT_PARTICLE,	// ������Ч
	GAME_ZORDER_BATTLE,				// ս������
	GAME_ZORDER_SUSPEND,			// ��ͣ����
	GAME_ZORDER_UI_PARTICLE,		// UI��Ч
	GAME_ZORDER_NEW_PLAYER,			// ���ֽ���
	GAME_ZORDER_KEYBOARD,			// ����������(���������ϲ�)
	GAME_ZORDER_NUM
};

enum  PlaneMoveTo
{
	NOT_MOVE,				// ����
	TOP,					// ��
	BOTTOM,					// ��
	LEFT,					// ��
	RIGHT,					// ��
};

//�л�����
enum
{
	enemy_type_monster = 0,		// С��
	enemy_type_boss				// BOSS
};

const int DesignWidth = 600;		//��Ʒֱ��ʿ��
const int DesignHight = 800;		//��Ʒֱ��ʸ߶�
const float DISTANCE_PER_FRAME = 0.016666668f; // 1/60���ƶ��ľ���

const float PLANE_START_X = 240.0F;	 //�ɻ�������Ļ�����λ��
const float PLANE_START_Y = -100.0f; //�ɻ�������Ļ�����λ��

// �������Ż��������
const std::string gPlayerInfo[] = {"airplane", "airplane2", "airplane3", "zuheji", "erhaoji"};
const int g_WingXPos[] = {-60, 60, -80, 80, -45, 45};
const int g_WingYPos[] = {-15, -15, 25, 25, 55, 55};
const int g_WingPos[] = {3, 3, 4, 4, 3, 3};


// �������
const int g_ScoreIdx[] = {3100400, 1930700, 1200700, 889800, 803200, 750200, 683000, 654300, 627000, 600000};

const std::string  g_FirstName[] = {"Mary", "Patricia", "Linda", "Barbara", "Elizabeth", "Jennifer", "Maria", "Susan", "Margaret", "Dorothy", "Lisa", "Nancy", "Karen", "Betty", "Helen", "Sandra", "Donna", "Carol", "Ruth", "Sharon", "Michelle", "Laura", "Sarah", "Kimberly", "Deborah", "Jessica", "Shirley", "Cynthia", "Angela", "Melissa", "Brenda", "Amy", "Anna", "Rebecca", "Virginia", "Kathleen", "Pamela", "Martha", "Debra", "Amanda", "Stephanie", "Carolyn", "Christine", "Marie", "Janet", "Catherine", "Frances", "Ann", "Joyce", "Diane", "Alice", "Julie", "Heather", "Teresa", "Doris", "Gloria", "Evelyn", "Jean", "Cheryl", "Mildred", "Katherine", "Joan", "Ashley", "Judith", "Rose", "Janice", "Kelly", "Nicole", "Judy", "Christina", "Kathy", "Theresa", "Beverly", "Denise", "Tammy", "Irene", "Jane", "Lori", "Rachel", "Marilyn", "Andrea", "Kathryn", "Louise", "Sara", "Anne", "Jacqueline", "Wanda", "Bonnie", "Julia", "Ruby", "Lois", "Tina", "Phyllis", "Norma", "Paula", "Diana", "Annie", "Lillian", "Emily", "Robin", "Peggy", "Crystal", "Gladys", "Rita", "Dawn", "Connie", "Florence", "Tracy", "Edna", "Tiffany", "Carmen", "Rosa", "Cindy", "Grace", "Wendy", "Victoria", "Edith", "Kim", "Sherry", "Sylvia", "Josephine", "Thelma", "Shannon", "Sheila", "Ethel", "Ellen", "Elaine", "Marjorie", "Carrie", "Charlotte", "Monica", "Esther", "Pauline", "Emma", "Juanita", "Anita", "Rhonda", "Hazel", "Amber", "Eva", "Debbie", "April", "Leslie", "Clara", "Lucille", "Jamie", "Joanne", "Eleanor", "Valerie", "Danielle", "Megan", "Alicia", "Suzanne", "Michele", "Gail", "Bertha", "Darlene", "Veronica", "Jill", "Erin", "Geraldine", "Lauren", "Cathy", "Joann", "Lorraine", "Lynn", "Sally", "Regina", "Erica", "Beatrice", "Dolores", "Bernice", "Audrey", "Yvonne", "Annette", "June", "Samantha", "Marion", "Dana", "Stacy", "Ana", "Renee", "Ida", "Vivian", "Roberta", "Holly", "Brittany", "Melanie", "Loretta", "Yolanda", "Jeanette", "Laurie", "Katie", "Kristen", "Vanessa", "Alma", "Sue", "Elsie", "Beth", "Jeanne"};

const std::string  g_SecondNmae[] = {"James", "John", "Robert", "Michael", "William", "David", "Richard", "Charles", "Joseph", "Thomas", "Christopher", "Daniel", "Paul", "Mark", "Donald", "George", "Kenneth", "Steven", "Edward", "Brian", "Ronald", "Anthony", "Kevin", "Jason", "Matthew", "Gary", "Timothy", "Jose", "Larry", "Jeffrey", "Frank", "Scott", "Eric", "Stephen", "Andrew", "Raymond", "Gregory", "Joshua", "Jerry", "Dennis", "Walter", "Patrick", "Peter", "Harold", "Douglas", "Henry", "Carl", "Arthur", "Ryan", "Roger", "Joe", "Juan", "Jack", "Albert", "Jonathan", "Justin", "Terry", "Gerald", "Keith", "Samuel", "Willie", "Ralph", "Lawrence", "Nicholas", "Roy", "Benjamin", "Bruce", "Brandon", "Adam", "Harry", "Fred", "Wayne", "Billy", "Steve", "Louis", "Jeremy", "Aaron", "Randy", "Howard", "Eugene", "Carlos", "Russell", "Bobby", "Victor", "Martin", "Ernest", "Phillip", "Todd", "Jesse", "Craig", "Alan", "Shawn", "Clarence", "Sean", "Philip", "Chris", "Johnny", "Earl", "Jimmy", "Antonio", "Danny", "Bryan", "Tony", "Luis", "Mike", "Stanley", "Leonard", "Nathan", "Dale", "Manuel", "Rodney", "Curtis", "Norman", "Allen", "Marvin", "Vincent", "Glenn", "Jeffery", "Travis", "Jeff", "Chad", "Jacob", "Lee", "Melvin", "Alfred", "Kyle", "Francis", "Bradley", "Jesus", "Herbert", "Frederick", "Ray", "Joel", "Edwin", "Don", "Eddie", "Ricky", "Troy", "Randall", "Barry", "Alexander", "Bernard", "Mario", "Leroy", "Francisco", "Marcus", "Micheal", "Theodore", "Clifford", "Miguel", "Oscar", "Jay", "Jim", "Tom", "Calvin", "Alex", "Jon", "Ronnie", "Bill", "Lloyd", "Tommy", "Leon", "Derek", "Warren", "Darrell", "Jerome", "Floyd", "Leo", "Alvin", "Tim", "Wesley", "Gordon", "Dean", "Greg", "Jorge", "Dustin", "Pedro", "Derrick", "Dan", "Lewis", "Zachary", "Corey", "Herman", "Maurice", "Vernon", "Roberto", "Clyde", "Glen", "Hector", "Shane", "Ricardo", "Sam", "Rick", "Lester", "Brent", "Ramon", "Charlie", "Tyler", "Gilbert", "Gene"};

const std::string  g_PreItem[] = { "1, 5", "2, 1", "3, 5", "4, 1", "5, 5", "6, 500", "7, 50", "8, 50", "9, 150", "10, 50", "11, 50", "12, 50" };
const std::string  g_PrePlayerWeapon[] = { "15,8", "16,30", "19,30", "34,100", "35,24", "57,8", "58,8", "59,8", "60,8", "61,8", "62,8", "63,26", "64,8", "65,8", "66,8", "67,8", "68,8", "69,8", "70,8", "71,8", "72,8", "73,8", "74,8", "75,8", "76,8", "77,71" };

const int		   g_PreBoss[] = { 22, 260, 115, 118, 116, 264, 504, 505, 506, 507, 508, 509 };
const std::string  g_PreEnemy1[] = { "2, 8", "4, 4", "5, 4", "6, 2", "7, 4", "8, 1", "9, 2", "10, 1", "13, 3", "16, 6", "17, 4", "18, 16", "19, 7", "23, 1", "24, 1", "25, 1", "26, 1", "27, 1", "28, 1", "29, 2", "30, 1", "501, 3"};
const std::string  g_PreEnemyWeapon1[] = { "3, 28", "4, 8", "6, 23", "8, 56", "9, 58", "10, 182", "13, 96", "20, 20", "21, 72", "44, 16", "46, 60" };

const std::string  g_PreEnemy2[] = { "2, 6", "4, 4", "5, 4", "7, 4", "19, 7", "25, 1", "26, 1", "31, 8", "32, 2", "33, 4", "37, 1", "38, 3", "41, 1", "42, 8", "45, 6", "46, 4", "47, 4" };
const std::string  g_PreEnemyWeapon2[] = { "4, 6", "5, 20", "6, 12", "20, 83", "43, 144", "47, 69", "55, 3" };

const std::string  g_PreEnemy3[] = { "5, 3", "7, 3", "25, 1", "26, 1", "33, 4", "37, 1", "41, 1", "52, 10", "54, 4", "55, 4", "56, 6", "59, 1", "60, 2", "68, 12", "510, 3", "511, 4", "512, 4", "513, 3", "514, 3", "515, 4", "516, 4", "517, 4", "518, 4", "519, 4", "520, 4", "521, 4" };
const std::string  g_PreEnemyWeapon3[] = { "4, 41", "5, 2", "6, 48", "9, 90", "11, 99", "20, 104", "24, 53" };

const std::string  g_PreEnemy4[] = { "5,2","7,3","23,2","33,5","52,7","118,1","510,4","511,4","512,4","513,4","514,3","522,8","523,2","524,1","525,10","526,12","527,1","528,1","529,1","530,3"};
const std::string  g_PreEnemyWeapon4[] = { "4,7", "5,2", "6,12", "9,121", "10,311", "20,210", "24,104"};

const std::string  g_PreEnemy5[] = { "5,3","7,2","23,2","25,1","26,1","33,5","52,7","81,9","82,1","510,4","511,4","512,4","513,4","514,4","522,8","523,2","526,12","527,1","528,1","529,1","531,4","532,4","533,6","534,2","535,1"};
const std::string  g_PreEnemyWeapon5[] = { "3,185", "4,11", "5,10", "6,8", "11,95", "20,22", "24,194", "36,100" };

const std::string  g_PreEnemy6[] = { "23,1","33,4","52,8","81,6","82,1","121,1","125,6","129,7","131,3","336,2","526,12","527,1","528,1","529,1","531,4","532,4","533,6" };
const std::string  g_PreEnemyWeapon6[] = { "4,10", "5,8", "11,87", "20,22", "24,72", "43,355", "46,40", "47,94", "54,8", "55,18" };

const std::string  g_PreEnemy7[] = { "23,1","138,3","140,3","142,3","144,4","146,2","147,1","148,4","150,3","156,7","174,1","175,2","526,12","527,1","528,1","529,1","531,4","532,4","533,6","539,2","540,12","541,6" };
const std::string  g_PreEnemyWeapon7[] = { "3,28", "4,22", "9,58", "10,110", "11,86", "13,78", "20,21", "21,36", "24,104", "50,116" };

const std::string  g_PreEnemy8[] = { "131,3","142,3","144,3","148,4","150,3","156,7","160,4","163,7","174,1","175,2","200,1","336,2","510,4","511,2","512,4","513,3","514,3","526,12","527,1","528,1","529,1","531,4","532,4","533,6","534,2","535,1","536,8","537,2" };
const std::string  g_PreEnemyWeapon8[] = { "4,20", "5,5", "6,14", "11,87", "20,24", "24,178", "43,144", "46,40", "47,248", "55,3" };

const std::string  g_PreEnemy9[] = { "23,2","33,5","81,8","82,1","163,6","201,4","202,1","203,4","221,1","526,12","527,1","528,1","529,1","534,2","535,1","539,2","540,12","541,6" };
const std::string  g_PreEnemyWeapon9[] = { "5,9", "6,4", "9,33", "10,47", "11,28", "20,104", "24,201", "50,129" };

const std::string  g_PreEnemy10[] = { "23,1","142,2","144,4","148,3","150,3","156,7","160,3","163,6","174,1","175,2","200,1","226,2","228,4","241,1","242,1","336,2","531,4","532,4","533,6","538,3" };
const std::string  g_PreEnemyWeapon10[] = { "4,24", "5,6", "9,166", "10,311", "11,117", "13,80", "20,210", "46,40" };

const std::string  g_PreEnemy11[] = { "160,18","162,1","163,11","164,1","167,5","168,2","169,7","170,1","171,3","173,3","200,2" };
const std::string  g_PreEnemyWeapon11[] = { "3,146", "5,19", "6,24" "20,152", "24,213" "36,132" };

const std::string  g_PreEnemy12[] = { "5,3","7,3","25,1","222,3","224,3","244,1","509,1" };
const std::string  g_PreEnemyWeapon12[] = { "3,140", "4,5", "9,82", "10,227", "20,210", "24,49", "36,132", "43,355", "47,128", "54,8", "55,18" };


// const std::string g_EffectPathName[]  = {"p0", "p0_2", "p1", "p2","loading"};
// const std::string g_EffectPathImage[] = {"particleAtlas0", "particleAtlas0", "particleAtlas1", "particleAtlas2","loading"};

const std::string g_AnimationNameNull = "";
const std::string g_AnimationName_g0 = "g_0";
const std::string g_AnimationName_g3 = "g_3";
const std::string g_AnimationName_g8 = "g_8";
const std::string g_AnimationName_g13 = "g_13";
//std::string g_AnimationName_g0 = "g_0";


//���ݴ洢����
enum DATA_TYPE
{
	DATA_DIAMOND_LEVEL = 0,	//��ʯ�ӳɵȼ�
	DATA_WING_LEVEL,		//�Ż�ǿ���ȼ�
	DATA_FIRE_LEVEL,		//����ǿ���ȼ�
	DATA_CRAZY_LEVEL,		//����ǿ���ȼ�
	DATA_SHIELD_LEVEL,		//����ǿ���ȼ�
	DATA_BOMB_LEVEL,		//�Ա���ǿ���ȼ�
	DATA_BOMB_NUM,			//ը������
	DATA_LIFE_NUM,			//��������
	DATA_SHIELD_NUM,		//��������
	DATA_KILLED_NUM,		//ɱ������
	DATA_DIAMOND_NUM,		//��ʯ����
	DATA_DIAMOND_TOTAL,		//�ۼƻ�ñ�ʯ
	DATA_DIAMOND_ODD,		//��ʯ������ͷ
	DATA_TOP_SCORE,			//��߷�
	DATA_WORLD_RANK,		//��������
	DATA_NEXT_MISSION,		//����ս�ؿ�
	DATA_NEXT_BOSS,			//����սboss�ؿ�
	DATA_GAME_WEEK,			//��ս��Ŀ
	DATA_MUSIC_ON,			//������������
	DATA_EFFECT_ON,			//������Ч
	DATA_FRESH_MAN,			//���ֱ�־
	DATA_FRESH_STEP,		//���ֲ���
	DATA_ACTIVATION_TYPE,	//��������
	DATA_HAVE_PLANE3,		//ӵ��3�ŷɻ�
	DATA_HAVE_PLANE4,		//ӵ��4�ŷɻ�
	DATA_SHOW_STORY,		//��ʾ��ͷ����
	DATA_LASTTIME_RECEIVE,	//�ϴ��콱ʱ��
	DATA_ALLNUM_RECEIVE,	//�콱����
	DATA_FIRST_CHARGE,		//�׳�

	DATA_MAX
};

//��Ч����
enum SoundName
{
	Mic_back = 1,
	Mic_bgm_boss,
	Mic_bgm_menu,
	Mic_bomb,
	Mic_boom_l,
	Mic_boom_m,
	Mic_boom_s,
	Mic_boom_user,
	Mic_boss_transform,
	Mic_bs1,
	Mic_bs2,
	Mic_bullet1,
	Mic_bullet2,
	Mic_button,
	Mic_count,
	Mic_crash,
	Mic_crystal,
	Mic_entry,
	Mic_laser,
	Mic_levelmax1,
	Mic_levelmax2,
	Mic_life,
	Mic_logo,
	Mic_mission_complete,
	Mic_mission_start,
	Mic_pause,
	Mic_planebigtosmall,
	Mic_planecomein,
	Mic_platrate,
	Mic_powerup,
	Mic_rush,
	Mic_score,
	Mic_select,
	Mic_sg,
	Mic_shield,
	Mic_strengthen,
	Mic_sure,
	Mic_transform,
	Mic_unlockrank,
	Mic_warning,
	Mic_word

};

// ��������
enum BuyType
{
	Buy_Null,
	Buy_Life,					// ����
	Buy_Shield,					// ����
	Buy_Bomb,					// ը��
	Buy_Diamond,				// ��ʯ
	Buy_ChaoZhi,				// ��ֵ
	Buy_TuHaoJin,				// ������
	Buy_Plane3,					// �ɻ�3
	Buy_Plane4,					// �ɻ�4
	Buy_Genuine,				// ����
	Buy_PartDiamond,			// ��ʯת������
	Buy_PartWing,				// �Ż�ǿ������
	Buy_PartFire,				// ����ǿ���ȼ�
	Buy_PartErupt,				// ����ǿ���ȼ�
	Buy_PartShield,				// ����ǿ���ȼ�
	Buy_PartSkill,				// ��ɱ��ǿ���ȼ�
	Buy_AllPartUp,				// һ������
	Buy_FirstCharge,			// �׳�
};

// ����ID
#define CHANNEL_TEST 0 			//����
#define CHANNLE_XINYINHE 1		//������
#define CHANNLE_ZHANGYU 2		//��������
#define CHANNLE_WUCAI 3			//���ʱ��
#define CHANNLE_HAIYU 4			//��������
#define CHANNLE_UPAY 5			//�ű�-�߷�

#define MISSION_COUNT_INDEX		35

// ת��UTF-8
inline const char* G2U(const char* str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	
	int len = MultiByteToWideChar(CP_ACP,0,str,-1,NULL,0);
	
	wchar_t* wstr = new wchar_t[len+1];
	
	memset(wstr,0,len+1);

	MultiByteToWideChar(CP_ACP,0,str,-1,wstr,len);

	len = WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);

	char* temp = new char[len +1];

	memset(temp,0,len+1);

	WideCharToMultiByte(CP_UTF8,0,wstr,-1,temp,len,NULL,NULL);
	if (wstr)
	{
		delete []wstr;
	}

	return temp;

#else

	return str;

#endif

}