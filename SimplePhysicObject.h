#ifndef __SIMPLE_PHYSIC_OBJECT_H__
#define __SIMPLE_PHYSIC_OBJECT_H__

#include "EngineConfig.h"

class SimplePhysicWorld;

class SimplePhysicObject
{
public:
	enum SPOT {
		SPOT_STATIC,
		SPOT_DYNAMIC,
		SPOT_PLATFORM,
		SPOT_SENSOR
	};
	enum SFT {
		SFT_NATURE_ENTITY,
		SFT_FRIENDLY_ENTITY,
		SFT_ENEMY_ENTITY,
		SFT_SENSOR_VIEW,
		SFT_SENSOR_ATTACK
	};
protected:
	cocos2d::DrawNode* debugDraw = NULL;
	//--------------------------------------
	SimplePhysicWorld* parent = NULL;
	//--------------------------------------
	cocos2d::Vec2 verlocity;
	cocos2d::Size size;
	cocos2d::Vec2 position;
	std::vector<cocos2d::Vec2> objectCollisionPoints;
	SFT objectFilterType;

public:

	SimplePhysicObject();
	~SimplePhysicObject();

	//----- debug -------
	bool onDebugMode = false;
	cocos2d::DrawNode* getDebugDraw() { return debugDraw; };
	//-------------------
	static SimplePhysicObject* create(SPOT _type);
	virtual bool init();
	virtual void updateForStatic(float dt,const std::vector<SimplePhysicObject*>& staticObjs);
	virtual void updateForPlatform(float dt, const std::vector<SimplePhysicObject*>& platformObjs);
	//-------------------
	SPOT type = SPOT::SPOT_DYNAMIC;
	void setPosition(cocos2d::Vec2 pos);
	void setPositionX(int X);
	void setPositionY(int Y);
	void setVelocityX(float X);
	void setVelocityY(float Y);
	cocos2d::Vec2 getVolocity(){ return this->verlocity; };
	cocos2d::Vec2 getPosition();
	cocos2d::Vec2 centerPoint;
	void setSize(cocos2d::Size _size);
	void setObjectFilterType(SFT _type) { objectFilterType = _type; };
	SFT getObjectFilterType(){ return this->objectFilterType; };
	void setCollisionPoints(std::vector<cocos2d::Vec2> _objectCollisionPoints){ this->objectCollisionPoints = _objectCollisionPoints; };
	cocos2d::Size getSize(){ return this->size; };
	void setDebugInfo(cocos2d::Size _size, cocos2d::Color4F _color, cocos2d::Node* parent);
	//-------------------
	void setParent(SimplePhysicWorld* _parent) { parent = _parent; };

	//////////////////////////////////////////////////////////////////////////
	bool isJump = false;
	bool onJumping = false;
	bool isOnPlatform = false;
};

#endif
