#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class MapLayer : public cocos2d::Layer
{
	MapLayer();
	~MapLayer();

public:

	CREATE_FUNC(MapLayer);

	virtual bool init();

	//���õ�ͼ���
	bool	SetMapId(int id);//0~5

	// ��ʼ��
	void	Init();
	
	// ��Ϸѭ��
	void Tick(float dt);

	// ��ͣ�ƶ�
	void Pause() { m_bPause = true;};
	
	// �����ƶ�
	void Continue() { m_bPause = false;};
	

private:	

	//��ȡ��ͼװ�β㲼���ļ�
	bool LoadMapJson(const std::string& fileName);

	Sprite*					m_pBackgroundSpritr_1;		// ����1
	Sprite*					m_pBackgroundSpritr_2;		// ����2

	Sprite*					m_pFogSprite1;				// ������
	Sprite*					m_pFogSprite2;				// ������

	Sprite*					m_pClouds1;					// �Ʋ�
	Sprite*					m_pClouds2;					// �Ʋ�

	int						m_nDecorateNum;				// װ������
	Sprite*					DecorateList[10];			// װ�β�

	
	int			m_nMapID;		//��ͼ���
	bool		m_bPause;		//��ͣ�ƶ�

};
