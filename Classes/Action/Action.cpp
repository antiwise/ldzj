#include "Action.h"
#include "Common/LXUtils.h"
#include "CCActionIntervalEx.h"
#include "GameDefine.h"
#include "ActionManager.h"
#include "2d/CCActionInterval.h"

LXAction::LXAction()
	: m_nIndex(0)
{
}

LXAction::~LXAction() 
{
}

int LXAction::GetYPos(int pos)
{

	return Utils().GetFixedY(Utils().GetDesignHeight() - pos);
}

int LXAction::GetXPos(int pos)
{
	//return pos - 100;
	return Utils().GetFixedX(pos);
}

void LXAction::AddScriptCommand(const std::string &value)
{
	m_Commands.push_back(value);
}

FiniteTimeAction* LXAction::ParseScript(int loop)
{
	if(loop == startIndex)		// 开始解析
	{
		m_nIndex = 0;
	}
	
	Vector<FiniteTimeAction*>		m_Actions;		// 动作
	do 
	{
		// 按行解析脚本
		std::string script = m_Commands[m_nIndex].substr(1, m_Commands[m_nIndex].length() -1);
		m_nIndex++;	
		
		// 拆分指令
		std::vector<std::string> vecCommand;
		Utils().split(script, vecCommand, " ");
		if (vecCommand[0] == "loop")		// 循环
		{
			int nLoopNum = atoi(vecCommand[1].c_str());
			
			m_Actions.pushBack(ParseScript(nLoopNum));
		}
		else if (vecCommand[0] != "{" && vecCommand[0] != "sync" ) 
		{
			if (vecCommand[0] == "endLoop")		// 循环结束 
			{
				if (loop == -1)
				{
					//return RepeatForever::create(Sequence::create(m_Actions));
					return Repeat::create(Sequence::create(m_Actions),100);
				}
				else
				{
					return Repeat::create(Sequence::create(m_Actions), loop);
				}
			}
			if (vecCommand[0] == "}")			// 语句块循环
			{
				return Sequence::create(m_Actions);
			}
			if (vecCommand[0] == "endSync")		// 结束同步
			{
				return Spawn::create(m_Actions);
			}

			FiniteTimeAction* pAction = CreateAction(vecCommand);
			if (pAction)
			{
				m_Actions.pushBack(pAction);
			}
		}
		else
		{
			m_Actions.pushBack(ParseScript(0));
		}

	} while(m_nIndex < m_Commands.size());

	
	return Sequence::create(m_Actions);
}

FiniteTimeAction* LXAction::CreateAction(std::vector<std::string> &CommandVec)
{
	// 指令
	std::string command = CommandVec[0];

	// 参数解析
	std::vector<std::string> vecParam;
	Utils().split(CommandVec[1], vecParam, ",");
	if (command == "sizeTo")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "sizeBy")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "moveTo")
	{
		return MoveTo::create( atoi( vecParam[2].c_str() ), Vec2( GetXPos( atoi( vecParam[0].c_str() ) ), GetYPos( atoi( vecParam[1].c_str() ) ) ) );
	}
	else if (command == "moveBy")
	{
		return MoveBy::create( atoi( vecParam[2].c_str() ), Vec2( GetXPos( atoi(vecParam[0].c_str() ) ), Utils().GetFixedY( -atoi(vecParam[1].c_str() ) ) ) );
	}
	else if (command == "arcMoveTo")
	{
		return arcMoveTo::create( atof( vecParam[4].c_str()), GetXPos( atoi( vecParam[0].c_str() ) ), Utils().GetFixedY( atoi( vecParam[1].c_str() ) ), atoi( vecParam[2].c_str() ), atoi( vecParam[3].c_str() ) );
	}
	else if (command == "arcMoveBy")
	{
		return arcMoveBy::create( atof( vecParam[4].c_str()), GetXPos( atoi( vecParam[0].c_str() ) ), Utils().GetFixedY( atoi( vecParam[1].c_str() ) ), atoi(vecParam[2].c_str() ), atoi( vecParam[3].c_str() ) );
	}
	else if (command == "scaleTo")
	{
		return ScaleTo::create( atoi( vecParam[2].c_str() ), atoi( vecParam[0].c_str() ), atoi( vecParam[1].c_str() ) ); 
	}
	else if (command == "scaleBy")
	{
		return ScaleBy::create( atoi( vecParam[2].c_str() ), atoi( vecParam[0].c_str() ), atoi( vecParam[1].c_str() ) );
	}
	else if (command == "rotateTo")
	{
		return RotateTo::create(atoi(vecParam[0].c_str()), atoi(vecParam[1].c_str()));
	}
	else if (command == "rotateBy")
	{
		return RotateBy::create(atoi(vecParam[0].c_str()), atoi(vecParam[1].c_str()));
	}
	else if (command == "color")
	{
		if (atoi(vecParam[0].c_str()) < 0)
		{
			return TintTo::create(atoi(vecParam[1].c_str()), 255, 255, 255);
		}
		else
		{
			return TintTo::create(atoi(vecParam[1].c_str()), 255, 0, 0);
		}
	}
	else if (command == "alpha")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "delay")
	{
		return DelayTime::create(atof(vecParam[0].c_str()));
	}
	else if (command == "touchable")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "visible")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "originTo")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "originToAnchor")
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "shoot")					
	{
		return  ShootData::create(CommandVec[1]);// 不知为什么不会触发update函数
		// 开火
		
		/*auto retAct = CallFunc::create([=](){
			auto act = ShootData::create(CommandVec[1]); 
			act->update(0);
		});
		return retAct;*/
			
	}
	else if (command == "shootAll")					
	{
		// 开火
		return ShootAll::create();
		/*auto retAct = CallFunc::create([](){
			auto act = ShootAll::create();
			act->update(0);
		});
		return retAct;*/
	}
	else if (command == "stop")
	{
		// 停止
		return DelayTime::create(0.1f);
	}
	else if (command == "stopAll")					
	{
		// 停止全部行为
		return StopAll::create();
	}
	else if (command == "checkHp")
	{
		return CheckValue::create(atoi(vecParam[0].c_str()));
	}
	else if (command == "changeAnimation")
	{
		return ChangeAnimation::create(vecParam[1], atoi(vecParam[2].c_str()));
	}
	else if (command == "nextAnimation")
	{
		return NextAnimation::create(vecParam[1]);
	}
	else if (command == "bossAI")
	{
		/*Action* act = CallFunc::create([=]()->Action*{
			auto ret = LXActionManager().getAct(vecParam[0], ai_tag_bossai);
			if (ret)
			{
			return ret;
			}
			return DelayTime::create(0.1);
			});*/
		
		/*Sequence* ret = dynamic_cast<Sequence*>(LXActionManager().getAct(vecParam[0], ai_tag_bossai));
		return ret;*/
		return  BossAI::create(vecParam[0]);


		//return BossAI::create(vecParam[0]);
	}
	else if (command == "changeHit")
	{
		return ChangeHit::create(atoi(vecParam[0].c_str()), atoi(vecParam[1].c_str()), atoi(vecParam[2].c_str()), atoi(vecParam[3].c_str()), atoi(vecParam[4].c_str()));
	}
	else if (command == "addEffect")
	{
		return AddEffect::create(vecParam[1], atoi(vecParam[1].c_str()), atoi(vecParam[2].c_str()), atoi(vecParam[3].c_str()));
	}
	else if (command == "addFollowEffect")
	{
		return AddFollowEffect::create(vecParam[1], atoi(vecParam[1].c_str()), atoi(vecParam[2].c_str()));
	}
	else if (command == "fireEffect")
	{
		std::vector<int> vec;
		for(int i = 1; i < vecParam.size(); i++)
		{
			vec.push_back(atoi(vecParam[i].c_str()));
		}
		
		return FireEffect::create(vecParam[1], vec);
	}
	else if (command == "userBullet")
	{
		bool bValue = atoi(vecParam[0].c_str()) == 0 ? true : false;
		return UserBullet::create(bValue);
	}
	else if (command == "moveShooter")				
	{
		return MoveShooter::create(atoi(vecParam[1].c_str()), atof(vecParam[2].c_str()), atof(vecParam[3].c_str()));
	}
	else if (command == "finish")					
	{
		// 停止动作
		bool value = atoi(vecParam[0].c_str()) == 0?true:false;
		return Finish::create(value);
	}
	else if (command == "sound")					// 尚未处理
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "music")					// 尚未处理
	{
		return DelayTime::create(0.1f);
	}
	else if (command == "teach")					// 尚未处理
	{
		return DelayTime::create(0.1f);
	}
	else
	{
		return DelayTime::create(0.1f);
	}
}