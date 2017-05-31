#include "cocos2d.h"

USING_NS_CC;

//自定义的一些动画

/**  Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several arcMoveBy actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class arcMoveBy : public ActionInterval
{
public:
    /** creates the action */
    static arcMoveBy* create(float duration, float x, float y, float xparam, float yparam);

    //
    // Overrides
    //
    virtual arcMoveBy* clone() const override;
	virtual arcMoveBy* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    arcMoveBy() {}
    virtual ~arcMoveBy() {}

    /** initializes the action */
    bool initWithDuration(float duration, float x, float y, float xparam, float yparam);

protected:
	float _lastTime;
	float _orignx;
	float _origny;
	float _x;
	float _y;
	float _xparam;
	float _yparam;
	float _param1;
	float _param2;
	float _param3;
	float _param4;
	float _param5;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(arcMoveBy);
};

/** Moves a Node object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
 Several arcMoveTo actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class arcMoveTo : public ActionInterval
{
public:
    /** creates the action */
    static arcMoveTo* create(float duration, float x, float y, float xparam, float yparam);

    //
    // Overrides
    //
    virtual arcMoveTo* clone() const override;
	virtual arcMoveTo* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    arcMoveTo() {}
    virtual ~arcMoveTo() {}

    /** initializes the action */
    bool initWithDuration(float duration, float x, float y, float xparam, float yparam);

protected:
	float _lastTime;
	float _targetOrignx;	// 原坐标X
	float _targetOrigny;	// 原坐标Y
	float _x;
	float _y;
	float _xparam;
	float _yparam;
	float _param1;
	float _param2;
	float _param3;
	float _param4;
	float _param5;
	float _param6;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(arcMoveTo);
};

/** CheckValue use for check some value is arrive the target
 @since 3.2 2014-8-8 by tony
 */
class CheckValue : public ActionInterval
{
public:
    /** creates the action */
    static CheckValue* create(int value);

    //
    // Overrides
    //
    virtual CheckValue* clone() const override;
	virtual CheckValue* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    CheckValue() {}
    virtual ~CheckValue() {}

    /** initializes the action */
    bool initWithDuration(int value);

protected:
	int		_value;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(CheckValue);
};

/** ChangeAnimation use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class ChangeAnimation : public ActionInterval
{
public:
    /** creates the action */
    static ChangeAnimation* create(const std::string &animation, int type);

    //
    // Overrides
    //
    virtual ChangeAnimation* clone() const override;
	virtual ChangeAnimation* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    ChangeAnimation() {}
    virtual ~ChangeAnimation() {}

    /** initializes the action */
    bool initWithDuration(const std::string &animation, int type);

protected:
	std::string _animation;
	int			_type;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ChangeAnimation);
};

/** NextAnimation use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class NextAnimation : public ActionInterval
{
public:
    /** creates the action */
    static NextAnimation* create(const std::string &animation);

    //
    // Overrides
    //
    virtual NextAnimation* clone() const override;
	virtual NextAnimation* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    NextAnimation() {}
    virtual ~NextAnimation() {}

    /** initializes the action */
    bool initWithDuration(const std::string &animation);

protected:
	std::string _animation;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(NextAnimation);
};

/** BossAI use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class BossAI : public ActionInterval
{
public:
    /** creates the action */
    static BossAI* create(const std::string &action);

    //
    // Overrides
    //
    virtual BossAI* clone() const override;
	virtual BossAI* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    BossAI() {}
    virtual ~BossAI() {}

    /** initializes the action */
    bool initWithDuration(const std::string &action);

protected:
	std::string _action;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(BossAI);
};

/** ChangeHit use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class ChangeHit : public ActionInterval
{
public:
    /** creates the action */
    static ChangeHit* create(int type, int a, int b, int c, int d);

    //
    // Overrides
    //
    virtual ChangeHit* clone() const override;
	virtual ChangeHit* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    ChangeHit() {}
    virtual ~ChangeHit() {}

    /** initializes the action */
    bool initWithDuration(int type, int a, int b, int c, int d);

protected:
	int			_type;
	int			_a;
	int			_b;
	int			_c;
	int			_d;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ChangeHit);
};

/**  AddEffect use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  AddEffect : public ActionInterval
{
public:
    /** creates the action */
    static  AddEffect* create(const std::string &effect, int value1, int value2, int value3);

    //
    // Overrides
    //
    virtual  AddEffect* clone() const override;
	virtual  AddEffect* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     AddEffect() {}
    virtual ~ AddEffect() {}

    /** initializes the action */
    bool initWithDuration(const std::string &effect, int value1, int value2, int value3);

protected:
	std::string _effect;
	int			_value1;
	int			_value2;
	int			_value3;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( AddEffect);
};

/**  AddFollowEffect use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  AddFollowEffect : public ActionInterval
{
public:
    /** creates the action */
    static  AddFollowEffect* create(const std::string &effect, int value1, int value2);

    //
    // Overrides
    //
    virtual  AddFollowEffect* clone() const override;
	virtual  AddFollowEffect* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     AddFollowEffect() {}
    virtual ~ AddFollowEffect() {}

    /** initializes the action */
    bool initWithDuration(const std::string &effect, int value1, int value2);

protected:
	std::string _effect;
	int			_value1;
	int			_value2;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( AddFollowEffect);
};

/**  FireEffect use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  FireEffect : public ActionInterval
{
public:
    /** creates the action */
    static  FireEffect* create(const std::string &effect, std::vector<int> &vec);

    //
    // Overrides
    //
    virtual  FireEffect* clone() const override;
	virtual  FireEffect* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     FireEffect() {}
    virtual ~ FireEffect() {}

    /** initializes the action */
    bool initWithDuration(const std::string &effect, std::vector<int> &vec);

protected:
	std::string			_effect;
	std::vector<int>	_vec;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( FireEffect);
};

/**  UserBullet use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  UserBullet : public ActionInterval
{
public:
    /** creates the action */
    static  UserBullet* create(bool value);

    //
    // Overrides
    //
    virtual  UserBullet* clone() const override;
	virtual  UserBullet* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     UserBullet() {}
    virtual ~ UserBullet() {}

    /** initializes the action */
    bool initWithDuration(bool value);

protected:
	int			_value;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( UserBullet);
};

/**  Finish use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  Finish : public ActionInterval
{
public:
    /** creates the action */
    static  Finish* create(bool value);

    //
    // Overrides
    //
    virtual  Finish* clone() const override;
	virtual  Finish* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     Finish() {}
    virtual ~ Finish() {}

    /** initializes the action */
    bool initWithDuration(bool value);

protected:
	bool _clean;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( Finish);
};

/**  Sound use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  Sound : public ActionInterval
{
public:
    /** creates the action */
    static  Sound* create(const std::string &sound);

    //
    // Overrides
    //
    virtual  Sound* clone() const override;
	virtual  Sound* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     Sound() {}
    virtual ~ Sound() {}

    /** initializes the action */
    bool initWithDuration(const std::string &sound);

protected:
	std::string _sound;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( Sound);
};

/**  Music use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  Music : public ActionInterval
{
public:
    /** creates the action */
    static  Music* create(const std::string &music);

    //
    // Overrides
    //
    virtual  Music* clone() const override;
	virtual  Music* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     Music() {}
    virtual ~ Music() {}

    /** initializes the action */
    bool initWithDuration(const std::string &music);

protected:
	std::string _music;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( Music);
};

/**  Teach use for change object Animation
 @since 3.2 2014-8-8 by tony
 */
class  Teach : public ActionInterval
{
public:
    /** creates the action */
    static  Teach* create(int index);

    //
    // Overrides
    //
    virtual  Teach* clone() const override;
	virtual  Teach* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
     Teach() {}
    virtual ~ Teach() {}

    /** initializes the action */
    bool initWithDuration(int index);

protected:
	int			_index;

private:
    CC_DISALLOW_COPY_AND_ASSIGN( Teach);
};

/**  Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several LXMoveBy actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class LXMoveBy : public Action
{
public:
    /** creates the action */
    static LXMoveBy* create(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse);

    //
    // Overrides
    //
    virtual LXMoveBy* clone() const override;
	virtual LXMoveBy* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual bool isDone(void) const override;
    virtual void step(float dt);
    
CC_CONSTRUCTOR_ACCESS:
    LXMoveBy() {}
    virtual ~LXMoveBy() {}

    /** initializes the action */
    bool initWithDuration(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse);

protected:
    float _speed;			  // 移动速度
    float _maxSpeed;		  // 最大移动速度
    float _minSpeed;		  // 最小移动速度
    float _degree;			  // 角度
    float _addSpeed;		  // 速度加成
    float _addDegree;		  // 角度变化
    float _topSpeedLimit;	  // 存在最大移动速度上限
    float _needReverse;		  // 是否需要反向减速移动

private:
    CC_DISALLOW_COPY_AND_ASSIGN(LXMoveBy);
};

/**  Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several LXMoveByScreen actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class LXMoveByRect : public LXMoveBy
{
public:
    /** creates the action */
    static LXMoveByRect* create(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse,
		int x,
		int y,
		int w,
		int h);

    //
    // Overrides
    //
    virtual LXMoveByRect* clone() const override;
	virtual LXMoveByRect* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float dt);
    
CC_CONSTRUCTOR_ACCESS:
    LXMoveByRect() {}
    virtual ~LXMoveByRect() {}

    /** initializes the action */
    bool initWithDuration(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse,
		int x,
		int y,
		int w,
		int h);

protected:
	int	_x;						// rect x
	int _y;						// rect y
	int _w;						// rect w
	int _h;						// rect h

private:
    CC_DISALLOW_COPY_AND_ASSIGN(LXMoveByRect);
};

/**  Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several LXShoot actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class LXShooterLogic;
class LXShoot : public Action
{
public:
    /** creates the action */
    static LXShoot* create(LXShooterLogic *pLogic,
		int group, 
		float groupWaitTime, 
		float groupDegreeDelta, 
		int num, 
		float waitTime, 
		float degreeDelta, 
		float speedDelta,
		float delay);

    //
    // Overrides
    //
    virtual LXShoot* clone() const override;
	virtual LXShoot* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual bool isDone(void) const override;
    virtual void step(float dt);
    
CC_CONSTRUCTOR_ACCESS:
    LXShoot() {}
    virtual ~LXShoot() {}

    /** initializes the action */
    bool initWithDuration(LXShooterLogic *pLogic,
		int group, 
		float groupWaitTime, 
		float groupDegreeDelta, 
		int num, 
		float waitTime, 
		float degreeDelta, 
		float speedDelta,
		float delay);

	float calcDegree();

protected:
	LXShooterLogic* _pLogic;	// 子弹发射逻辑
    float	_pastTime;			// 运行时长
	bool	_done;				// 所有子弹发射完毕
	float	_delay;				// 延迟
	float	_degree;			// 角度
    float	_speed;				// 速度
	float	_speedDelta;		// 速度加成
	float	_targetDegree;		// 最终角度
	float	_targetSpeed;		// 最终速度
	int		_num;				// 数量
	int		_targetNum;			// 当前数量
	float	_waitTime;			// 等待时间
	float	_degreeDelta;		// 角度加成
	int		_group;				// 组
	float	_groupWaitTime;		// 组间等待时间
	float	_groupDegreeDelta;	// 组间角度变化
	bool	_groupInit;			// 组初始化状态

private:
    CC_DISALLOW_COPY_AND_ASSIGN(LXShoot);
};

/** ShootData use for change object shoot src
 @since 3.2 2014-8-8 by tony
 */
class ShootData : public ActionInterval
{
public:
    /** creates the action */
    static ShootData* create(const std::string &shootData);

    //
    // Overrides
    //
    virtual ShootData* clone() const override;
	virtual ShootData* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    ShootData() {}
    virtual ~ShootData() {}

    /** initializes the action */
    bool initWithDuration(const std::string &shootData);

protected:
	std::string _shootData;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ShootData);
};

/** ShootAll use for change object shoot src
 @since 3.2 2014-8-8 by tony
 */
class ShootAll : public ActionInterval
{
public:
    /** creates the action */
    static ShootAll* create();

    //
    // Overrides
    //
    virtual ShootAll* clone() const override;
	virtual ShootAll* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    ShootAll() {}
    virtual ~ShootAll() {}

    /** initializes the action */
    bool initWithDuration();

protected:

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ShootAll);
};

/** StopAll use for change object shoot src
 @since 3.2 2014-8-8 by tony
 */
class StopAll : public ActionInterval
{
public:
    /** creates the action */
    static StopAll* create();

    //
    // Overrides
    //
    virtual StopAll* clone() const override;
	virtual StopAll* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    StopAll() {}
    virtual ~StopAll() {}

    /** initializes the action */
    bool initWithDuration();

protected:

private:
    CC_DISALLOW_COPY_AND_ASSIGN(StopAll);
};

/** MoveShooter use for change object shoot src
 @since 3.2 2014-8-8 by tony
 */
class MoveShooter : public ActionInterval
{
public:
    /** creates the action */
    static MoveShooter* create(int index, float x, float y);

    //
    // Overrides
    //
    virtual MoveShooter* clone() const override;
	virtual MoveShooter* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    MoveShooter() {}
    virtual ~MoveShooter() {}

    /** initializes the action */
    bool initWithDuration(int index, float x, float y);

protected:
	int _index;
	float _x;
	float _y;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveShooter);
};


/**  Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several LXMoveByScreen actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */
class LXObject;
class LXMoveTrack : public LXMoveBy
{
public:
    /** creates the action */
    static LXMoveTrack* create(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse,
		float delta,
		float maxDegreeDelta,
		LXObject* pTarget,
		int x,
		int y);

    //
    // Overrides
    //
    virtual LXMoveTrack* clone() const override;
	virtual LXMoveTrack* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float dt);
    
public:
	LXObject*		GetTarget() { return _pTarget; }
	float			GetOffsetX() { return _x; }
	float			GetOffsetY() { return _y; }

CC_CONSTRUCTOR_ACCESS:
    LXMoveTrack() {}
    virtual ~LXMoveTrack() {}

    /** initializes the action */
    bool initWithDuration(float speed, 
		float maxSpeed, 
		float minSpeed, 
		float degree, 
		float addSpeed, 
		float addDegree, 
		bool topSpeedLimit,
		bool needReverse,
		float delta,
		float maxDegreeDelta,
		LXObject* pTarget,
		int x,
		int y);

protected:
	float		_delta;				// 时长
	float		_maxDegreeDelta;	// 最大变化角度
	LXObject*	_pTarget;			// 目标
	int			_x;					// OffsetX
	int			_y;					// OffsetY
	bool		_bHasTarget;		// 是否存在目标
	Rect		_rect;				// 包围盒信息

private:
    CC_DISALLOW_COPY_AND_ASSIGN(LXMoveTrack);
};
