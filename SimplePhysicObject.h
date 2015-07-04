#ifndef __SIMPLE_PHYSIC_OBJECT_H__
#define __SIMPLE_PHYSIC_OBJECT_H__

#include "EngineConfig.h"

class SimplePhysicWorld;

class SimplePhysicObject
{
protected:
	cocos2d::DrawNode* debugDraw = NULL;
	//--------------------------------------
	SimplePhysicWorld* parent = NULL;
	//--------------------------------------
	cocos2d::Vec2 verlocity;
	cocos2d::Size size;
	cocos2d::Vec2 position;
	std::vector<cocos2d::Vec2> objectCollisionPoints;
public:
	enum SPOT {
		SPOT_STATIC,
		SPOT_DYNAMIC,
		SPOT_PLATFORM
	};

	SimplePhysicObject();
	~SimplePhysicObject();

	//----- debug -------
	bool onDebugMode = false;
	cocos2d::DrawNode* getDebugDraw() { return debugDraw; };
	//-------------------
	static SimplePhysicObject* create(SPOT _type);
	virtual bool init();
	virtual void update(float dt,const std::vector<SimplePhysicObject*>& staticObjs);
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
	cocos2d::Size getSize(){ return this->size; };
	void setDebugInfo(cocos2d::Size _size, cocos2d::Color4F _color, cocos2d::Node* parent);
	//-------------------
	void setParent(SimplePhysicWorld* _parent) { parent = _parent; };

	//////////////////////////////////////////////////////////////////////////
	bool isJump = false;
	bool onJumping = false;
};

#endif
