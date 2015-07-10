#ifndef __SIMPLE_PHYSIC_WORLD_H__
#define __SIMPLE_PHYSIC_WORLD_H__

#include "EngineConfig.h"
#include "SimplePhysicObject.h"

class SimplePhysicWorld
{
private:
	//----- store temp all child
	std::vector<SimplePhysicObject*> allChildrenTemp;
	bool isDirty = false;
	//--------------------
	std::vector<SimplePhysicObject*> childrenDynamic;
	std::vector<SimplePhysicObject*> childrenStatic;
	std::vector<SimplePhysicObject*> childrenPlatform;
	cocos2d::Vec2 gravity;
	cocos2d::Node* debugDrawer = NULL;
public:
	SimplePhysicWorld();
	~SimplePhysicWorld();
	//-----------------------
	static SimplePhysicWorld* create();
	bool init();
	void update(float dt);
	//-----------------------
	void setDebugDisplay(cocos2d::Node* _debugDrawer){ this->debugDrawer = _debugDrawer; };
	//-----------------------
	void setGravity(cocos2d::Vec2 _gravity){gravity = _gravity;};
	cocos2d::Vec2 getGravity(){ return this->gravity; };
	//-----------------------
	void addChild(SimplePhysicObject* child);
	//-----------------------
	std::vector<SimplePhysicObject*> getAllChildren();
	std::vector<SimplePhysicObject*> getAllChildrenWithFilter(std::vector<SimplePhysicObject::SFT> _filter);
};

#endif
