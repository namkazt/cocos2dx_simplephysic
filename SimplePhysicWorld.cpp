#include "SimplePhysicWorld.h"

USING_NS_CC;

SimplePhysicWorld::SimplePhysicWorld()
{
}


SimplePhysicWorld::~SimplePhysicWorld()
{
	for (int i = 0; i < this->childrenDynamic.size(); i++){
		delete this->childrenDynamic[i];
	}
	this->childrenDynamic.clear();
	for (int i = 0; i < this->childrenStatic.size(); i++){
		delete this->childrenStatic[i];
	}
	this->childrenStatic.clear();
	for (int i = 0; i < this->childrenPlatform.size(); i++){
		delete this->childrenPlatform[i];
	}
	this->childrenPlatform.clear();
}


SimplePhysicWorld* SimplePhysicWorld::create()
{
	SimplePhysicWorld* ref = new (std::nothrow) SimplePhysicWorld();
	if (ref && ref->init()){
		return ref;
	}
	else{
		delete ref;
		ref = NULL;
		return ref;
	}
}

bool SimplePhysicWorld::init()
{

	return true;
}

void SimplePhysicWorld::update(float dt)
{
	for (int i = 0; i < this->childrenDynamic.size(); i++)
	{
		this->childrenDynamic[i]->update(dt,this->childrenStatic);
	}
}

//-------------------------------------------------------
void SimplePhysicWorld::addChild(SimplePhysicObject* child)
{
	child->setParent(this);
	if (child->type == SimplePhysicObject::SPOT_DYNAMIC){
		this->childrenDynamic.push_back(child);
		if (this->debugDrawer != NULL) child->setDebugInfo(child->getSize(), Color4F::RED, this->debugDrawer);
	}
	else if(child->type == SimplePhysicObject::SPOT_STATIC){
		this->childrenStatic.push_back(child);
		if (this->debugDrawer != NULL) child->setDebugInfo(child->getSize(), Color4F::BLUE, this->debugDrawer);
	}
	else if (child->type == SimplePhysicObject::SPOT_PLATFORM){
		this->childrenPlatform.push_back(child);
		if (this->debugDrawer != NULL) child->setDebugInfo(child->getSize(), Color4F::GREEN, this->debugDrawer);
	}
}