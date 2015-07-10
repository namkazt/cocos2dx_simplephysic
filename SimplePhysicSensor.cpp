#include "SimplePhysicSensor.h"
#include "SimplePhysicWorld.h"

USING_NS_CC;

SimplePhysicSensor::SimplePhysicSensor()
{
	SimplePhysicObject::SimplePhysicObject();
}


SimplePhysicSensor::~SimplePhysicSensor()
{
	SimplePhysicObject::~SimplePhysicObject();
}

SimplePhysicSensor* SimplePhysicSensor::create()
{
	SimplePhysicSensor* ref = new (std::nothrow) SimplePhysicSensor();
	ref->onDebugMode = false;
	if (ref && ref->init()){
		return ref;
	}
	else{
		delete ref; 
		ref = NULL;
		return ref;
	}
}

bool SimplePhysicSensor::init()
{
	if (!SimplePhysicObject::init()){
		return false;
	}
	return true;
}

void SimplePhysicSensor::update(float dt)
{
	//---------- check contact objects first if have
	for (int i = 0; i < this->filterContactObjects.size(); i++)
	{
		//--TODO check contact
	}
	//---------- check contact for filter type
	if (this->filterContactType.size() > 0)
	{
		std::vector<SimplePhysicObject*> childWithFilter = this->parent->getAllChildrenWithFilter(this->filterContactType);
		for (int i = 0; i < childWithFilter.size(); i++)
		{
			//--TODO check contact
		}
	}
}

