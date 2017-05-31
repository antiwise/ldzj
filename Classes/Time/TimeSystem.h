#pragma once
#include <string>

class CTimeSystem
{
public:
	CTimeSystem();
	//ȡ�õ�ǰʱ��
	unsigned int						GetTimeNow(void) const { return m_dwTimeNow; }

public:
	//�߼���ѭ����
	virtual void						Tick();
    //ȡ��ǰ�߼�֡��ʱ���
	virtual unsigned int				GetFrameTime();
	//����ʱ��
	virtual unsigned int				CalSubTime(unsigned int timeStart, unsigned int timeEnd);
	//������һ֡����ʱ��
	virtual unsigned int				GetLastFrameTime();
	//��ȡʱ��
	virtual std::string					GetDateTimeAsString();
	//��ȡʱ��
	virtual unsigned int				GetDateTime();
	//��ȡ�߼�֡��
	virtual unsigned int				GetFrame() { return m_nFrame; }
	//��ȡ�߼�����ʱ��
	virtual unsigned int				GetPastTime() { return m_dwPastTime; }
	//����ʱ�ӱ���
	virtual void						SetTimeScale(float scale) { m_fTimeScale = scale; }

private:
	unsigned int		m_dwTimeNow;		//��ǰʱ��
	unsigned int		m_dwLastFrameTime;	//���һ֡���ʱ��
	unsigned int		m_dwPastTime;		//�ӷ�������������ǰ������ʱ��
	unsigned int		m_nFrame;			//�߼�֡����
	float				m_fTimeScale;		//ʱ�ӱ���
};

CTimeSystem &TimeSystem();