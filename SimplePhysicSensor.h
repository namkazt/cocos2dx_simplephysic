#ifndef __SIMPLE_PHYSIC_SENSOR_H__
#define __SIMPLE_PHYSIC_SENSOR_H__

#include "EngineConfig.h"
#include "SimplePhysicObject.h"

class SimplePhysicSensor : public SimplePhysicObject
{
protected:
	std::vector<SimplePhysicObject*> filterContactObjects;
	std::vector<SFT> filterContactType;

public:

	SimplePhysicSensor();
	~SimplePhysicSensor();

	static SimplePhysicSensor* create();
	virtual bool init();

	void update(float dt);

	void setSensorFilterType(std::vector<SFT> _filterContactType){ this->filterContactType = _filterContactType; };
	void setSensorFilterObjects(std::vector<SimplePhysicObject*> _filterContactObjects) { this->filterContactObjects = _filterContactObjects; };
};

#endif
