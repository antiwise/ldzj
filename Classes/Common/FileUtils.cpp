#include "FileUtils.h"
#include "cocos2d.h"

USING_NS_CC;



std::string LXFileUtils::GetFileByName(std::string pFileName)
{  
	//��һ�Ȼ�ȡ�ļ���·��  
	std::string path = FileUtils::sharedFileUtils()->getWritablePath() + pFileName;  
	log("path = %s",path.c_str());  

	//����һ���ļ�ָ��  
	FILE* file = fopen(path.c_str(), "rb");  

	if (file) 
	{  
		char* buf;  //Ҫ��ȡ���ַ���  
		int len;    //��ȡ�ĳ���  
		/*��ȡ����*/  
		fseek(file, 0, SEEK_END);   //�Ƶ�β��  
		len = ftell(file);          //��ȡ����  
		rewind(file);               //�ع�ԭλ  
		log("count the file content len = %d",len);  
		//����buf�ռ�  
		buf = (char*)malloc(sizeof(char) * (len + 1));  
		if (!buf) {  
			log("malloc space is not enough.");  
			return NULL;  
		}  
		memset(buf, 0, len + 1);

		//��ȡ�ļ�  
		//��ȡ����buf����λ��С�����ȣ��ļ�ָ��  
		int rLen = fread(buf, sizeof(char), len, file);  
		log("has read Length = %d",rLen);
		log("has read content = %s",buf);


		////������
		//char szKey[64];
		//memset(szKey, 0, 64);
		//sprintf(szKey, ".$UHGW&*IV786r7!");
		//encrypt(buf, len, szKey, 0);

		std::string result = buf;  
		fclose(file);  
		free(buf);  
		return result;  
	}  
	else 
	{
		log("open file error.");  
	}


	return "";  
}  

bool LXFileUtils::SaveFile(const char *pContent, std::string pFileName)
{  
	//��һ��ȡ������ļ�·��  
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;  
	log("save file path = %s",path.c_str());  
	
	//������
	char TempData[10240] = { 0 };
	strncpy(TempData, pContent, strlen(pContent));
	char* pTempEncrypt = TempData;
	/*int nSize = strlen(pTempEncrypt);
	char szKey[64];
	memset(szKey, 0, 64);
	sprintf(szKey, ".$UHGW&*IV786r7!");
	encrypt(pTempEncrypt, nSize, szKey, 0);*/

	//����һ���ļ�ָ��  
	//·����ģʽ  
	FILE* file = fopen(path.c_str(), "wb");  
	if (file) { 
		fwrite(pTempEncrypt,sizeof(char),strlen(pContent),file);
		//fputs(pTempEncrypt, file);  
		fclose(file);  
	}  
	else 
	{
		log("save file error.");  
	}
		
	return false;  
}  

//�ַ������
int		LXFileUtils::ConvertStringToVector(const char *strSource, std::vector< std::string >& vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();

	//------------------------------------------------------------
	//�Ϸ���
	if(!strSource || strSource[0] == '\0') return 0;

	std::string strSrc = strSource;

	//------------------------------------------------------------
	//���ҵ�һ���ָ��
	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(true)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));

		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}

void LXFileUtils::GetEffectFullPath(std::string &key)
{
	key = "particle/" + key;
}

void LXFileUtils::GetTexturePath(std::string &key)
{
	key = "Texture/Res/" + key;
}
void LXFileUtils::GetTexturePathLeijie(std::string &key)
{
	key = "/data/data/com.laser.game.pili/files/"+key;
}
void LXFileUtils::GetAnimationFullPath(std::string &key)
{
	key = "animation/" + key;	
}

void LXFileUtils::GetActionFullPath(std::string &key)
{
	key = "action/" + key;	
}

void LXFileUtils::GetMapFullPath(std::string &key)
{
	key = "Map/" + key;
}

void LXFileUtils::GetMapBgFullPath(std::string &key)
{
	key = "Texture/Res/scene/" + key;
}

void LXFileUtils::GetShooterFullPath(std::string &key)
{
	key = "shooters/" + key;
}

void LXFileUtils::GetParticleFullPath(std::string &key)
{
	key = "Texture/Res/particleAtlas0/" + key;
}

void LXFileUtils::GetBloodBarFullPath(std::string &key)
{
	key = "Texture/Res/ui/play/" + key;
}

void LXFileUtils::Get_UI_Json_FullPath( std::string &key )
{
	if ( key.empty() )
	{
		key =  "";
	}

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if ( !glview )
	{
		key =  "";
	}

	Size _Design = glview->getDesignResolutionSize();

	std::stringstream _Path;
	_Path <<"Texture/"<<key<<_Design.width<<_Design.height<<".ExportJson";

	key = _Path.str();
}

void LXFileUtils::AddSpriteFrameCache(const std::string &plist, const std::string &png)
{
	std::string plistFile = plist;
	std::string pngFile = png;
	GetTexturePath(plistFile);
	GetTexturePath(pngFile);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile, pngFile);
	Texture2D *pTexture = TextureCache::sharedTextureCache()->getTextureForKey(pngFile);
}

void LXFileUtils::encrypt(char *x, size_t xlen, char* KEY, size_t BeginPlace)
{ 
	if( (x)==NULL)
		return;

	char* t_pBuffer = (x); 
	char* pKey = {KEY}; 
	unsigned int KeyLen = (unsigned int)strlen(pKey); 
	for (unsigned int i = 0; i < (xlen); i++) 
	{ 
		*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen]; 
		t_pBuffer++; 
	} 
}