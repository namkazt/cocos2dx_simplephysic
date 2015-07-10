#include "SimplePhysicObject.h"
#include "SimplePhysicWorld.h"

USING_NS_CC;

SimplePhysicObject::SimplePhysicObject()
{
}


SimplePhysicObject::~SimplePhysicObject()
{
	if (debugDraw != NULL) debugDraw->release();
	verlocity = NULL;
	size = NULL;
	objectCollisionPoints.clear();
}

SimplePhysicObject* SimplePhysicObject::create(SPOT _type)
{
	SimplePhysicObject* ref = new (std::nothrow) SimplePhysicObject();
	ref->type = _type;
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
bool SimplePhysicObject::init()
{
	this->objectFilterType = SFT::SFT_NATURE_ENTITY;
	//-- init for debug method
	debugDraw = DrawNode::create();
	//-- init object info
	verlocity = { 0, 0 };
	return true;
}

void SimplePhysicObject::updateForPlatform(float dt, const std::vector<SimplePhysicObject*>& platformObjs)
{
	if (this->type == SPOT::SPOT_DYNAMIC){
		if (isJump) {
			isOnPlatform = false;
			return;
		}
		for (int i = 0; i < platformObjs.size(); i++)
		{
			if (round(this->position.x / 64) == round(platformObjs[i]->getPosition().x / 64) && round(this->position.y / 64) == round(platformObjs[i]->getPosition().y / 64))
			{
				isOnPlatform = true;
				return;
			}else
			{
				if (round(this->position.y / 64) - 1 >= 0){
					if (round(this->position.x / 64) == round(platformObjs[i]->getPosition().x / 64) && round(this->position.y / 64) - 1 == round(platformObjs[i]->getPosition().y / 64))
					{
						isOnPlatform = true;
						return;
					}
				}
			}
		}
		isOnPlatform = false;
	}
}

void SimplePhysicObject::updateForStatic(float dt, const std::vector<SimplePhysicObject*>& staticObjs)
{
	if (this->type == SPOT::SPOT_DYNAMIC){
		//if (isOnPlatform) return;
		//--- flag to check what kind if collision has occurred
		bool collideX = true, collideBottom = true, collideTop = true;

		for (int iteration = 0; iteration < 3 && (collideX || collideTop || collideBottom); iteration++){
			//------------ for each iterating
			std::set<int> boundObjects;
			//--- get next position
			Vec2 nextPosition = this->getPosition();
			nextPosition.add(verlocity);
			//--- create next position bounding box
			Rect nextPositionRect = Rect(
				std::min(this->getPosition().x, nextPosition.x),
				std::min(this->getPosition().y, nextPosition.y),
				size.width,
				size.height
				);
			//--- check each object to see whether it intersects with this object's bounding box
			for (int i = 0; i < staticObjs.size(); i++)
			{
				//--- create bounding box for this static object
				Rect staticObjRect = Rect(staticObjs[i]->getPosition().x, staticObjs[i]->getPosition().y, staticObjs[i]->getSize().width, staticObjs[i]->getSize().height);
				//--- create bounding box that used to check collide
				Rect bounds = Rect(nextPositionRect);
				bounds.merge(staticObjRect);
				//--- If a bnounding box intersects with this object's bounding box, add the geometry to to list to check for collisions
				if (bounds.getMaxX() - bounds.getMinX() < staticObjRect.getMaxX() - staticObjRect.getMinX() + nextPositionRect.getMaxX() - nextPositionRect.getMinX() &&
					bounds.getMaxY() - bounds.getMinY() < staticObjRect.getMaxY() - staticObjRect.getMinY() + nextPositionRect.getMaxY() - nextPositionRect.getMinY())
				{
					//-- add this index to bound objects to check collision
					boundObjects.insert(i);
				}
			}

			//-- No collisions found yet
			collideX = collideTop = collideBottom = false;
			//---------------------------------------------------
			float projectedMoveX, projectedMoveY, originalMoveX, originalMoveY, nextMoveX, nextMoveY;
			float vectorLength;
			int segments;
			//--- store original move 
			originalMoveX = nextPosition.x - this->getPosition().x;
			originalMoveY = nextPosition.y - this->getPosition().y;
			nextMoveX = originalMoveX;
			nextMoveY = originalMoveY;
			//-- iterate over each object whose boudning box intersects with this object's bounding box
			//-- until a collision is found
			for (auto it = boundObjects.begin(); it != boundObjects.end() && !collideX && !collideTop && !collideBottom; ++it)
			{
				//----------------------------------------------------
				// Speculative contacts section
				//----------------------------------------------------
				//-- get this static rect
				Rect staticObjRect = Rect(staticObjs[*it]->getPosition().x, staticObjs[*it]->getPosition().y, staticObjs[*it]->getSize().width, staticObjs[*it]->getSize().height);
				Rect specIntersection;
				specIntersection = CCRectMake(std::max(nextPositionRect.getMinX(), staticObjRect.getMinX()), std::max(nextPositionRect.getMinY(), staticObjRect.getMinY()), 0, 0);
				specIntersection.size.width = std::min(nextPositionRect.getMaxX(), staticObjRect.getMaxX()) - specIntersection.getMinX();
				specIntersection.size.height = std::min(nextPositionRect.getMaxY(), staticObjRect.getMaxY()) - specIntersection.getMinY();
				if (specIntersection.getMaxY() - specIntersection.getMinY() >= 1 && specIntersection.getMaxX() - specIntersection.getMinX() >= 1)
				{
					//-- Test four possible directions 
					//-- dir: 0 = top, 1 = bottom, 2 = left, 3 = right
					for (int dir = 0; dir < 4; dir++)
					{
						// skip test if the expected direction of movement make the test irrelevent
						//-----------------------------------------------------------------------
						if (dir == 0 && originalMoveY < 0) continue;
						if (dir == 1 && originalMoveY > 0) continue;
						if (dir == 2 && originalMoveX > 0) continue;
						if (dir == 3 && originalMoveX < 0) continue;

						float safeMoveX = std::max(std::max(specIntersection.getMinX() - (this->getPosition().x + size.width), this->getPosition().x - specIntersection.getMaxX()), 0.0f);
						float safeMoveY = std::max(std::max(specIntersection.getMinY() - (this->getPosition().y + size.height), this->getPosition().y - specIntersection.getMaxY()), 0.0f);
						float safeVecLen = std::sqrt(safeMoveX * safeMoveX + safeMoveY * safeMoveY);
						//-- calculate the lehgth of the movement vector using pythagoras
						vectorLength = std::sqrt(originalMoveX * originalMoveX + originalMoveY* originalMoveY);
						//-- our current position along the anticipated movement vector of this object in this frame
						projectedMoveX = originalMoveX * (safeMoveX / vectorLength);
						projectedMoveY = originalMoveY * (safeMoveY / vectorLength);
						//-- update vector lenght
						vectorLength -= safeVecLen;
						segments = 0;
						//-- we advence along the vector until it intersect with some box
						while (!staticObjRect.containsPoint(Vec2(
							static_cast<int>(objectCollisionPoints[dir * 2].x + this->getPosition().x + projectedMoveX),
							static_cast<int>(objectCollisionPoints[dir * 2].y + this->getPosition().y + projectedMoveY)))
							&&
							!staticObjRect.containsPoint(Vec2(
							static_cast<int>(objectCollisionPoints[dir * 2 + 1].x + this->getPosition().x + projectedMoveX),
							static_cast<int>(objectCollisionPoints[dir * 2 + 1].y + this->getPosition().y + projectedMoveY)))
							&&
							segments < vectorLength)
						{
							projectedMoveX += originalMoveX / vectorLength;
							projectedMoveY += originalMoveY / vectorLength;
							segments++;
						}
						//--- if an intersection occurred
						if (segments < vectorLength){
							//--- apply correction for over-movemnt
							if (segments > 0){
								projectedMoveX -= originalMoveX / vectorLength;
								projectedMoveY -= originalMoveY / vectorLength;
							}
							//--- adjust the X or Y component of the vector depending on
							//--- which direction we are currently testing
							if (dir >= 2 && dir <= 3) nextMoveX = projectedMoveX;
							if (dir >= 0 && dir <= 1) nextMoveY = projectedMoveY;
						}

					}
				}

				//----------------------------------------------------
				// Penetration resolution
				//----------------------------------------------------
				Rect objectNextBound = Rect(
					static_cast<int>(this->getPosition().x + nextMoveX),
					static_cast<int>(this->getPosition().y + nextMoveY),
					size.width,
					size.height
					);
				//--- get intersection of new bounding box to static box
				Rect intersection;
				intersection = CCRectMake(std::max(objectNextBound.getMinX(), staticObjRect.getMinX()), std::max(objectNextBound.getMinY(), staticObjRect.getMinY()), 0, 0);
				intersection.size.width = std::min(objectNextBound.getMaxX(), staticObjRect.getMaxX()) - intersection.getMinX();
				intersection.size.height = std::min(objectNextBound.getMaxY(), staticObjRect.getMaxY()) - intersection.getMinY();
				//--- convert to int to prevent juddering due to floating point dounding errors
				int intX = static_cast<int>(intersection.getMaxX() - intersection.getMinX());
				int intY = static_cast<int>(intersection.getMaxY() - intersection.getMinY());
				for (int dir = 0; dir < 4; dir++)
				{
					if (intersection.containsPoint(Vec2(
						static_cast<int>(objectCollisionPoints[dir * 2].x + this->getPosition().x + nextMoveX),
						static_cast<int>(objectCollisionPoints[dir * 2].y + this->getPosition().y + nextMoveY)))
						||
						intersection.containsPoint(Vec2(
						static_cast<int>(objectCollisionPoints[dir * 2 + 1].x + this->getPosition().x + nextMoveX),
						static_cast<int>(objectCollisionPoints[dir * 2 + 1].y + this->getPosition().y + nextMoveY)))
						)
					{
						switch (dir)
						{
						case 0: nextMoveY -= intY; break;
						case 1: nextMoveY += intY; break;
						case 2: nextMoveX += intX; break;
						case 3: nextMoveX -= intX; break;
						}
					}
				}

				//--- Detect what type of contact has occurred based on a comparison of the original
				if (nextMoveY > originalMoveY && originalMoveY < 0) collideBottom = true;
				if (nextMoveY < originalMoveY && originalMoveY > 0) collideTop = true;
				if (std::abs(nextMoveX - originalMoveX) > 0.01f) collideX = true;

				//--- in this case object cant jump because hit side of something
				//--- must fall down instead
				//--- without this object will still continue trying to travel upwards ( for jump thought behaviour )
				if (collideX && collideTop && verlocity.y > 0)
				{
					verlocity.y = 0;
				}
			}

			//-- if a contact has been deteced, apply the recalculate movement vector
			if (collideBottom || collideTop){
				this->setPositionY(this->getPosition().y + nextMoveY);
				verlocity.y = 0;

				if (collideBottom){
					//TODO: set jumping = false here to reset jump
					if (isJump)
					{
						isJump = false;
						verlocity.x = 0;
					}
				}
			}
			if (collideX){
				this->setPositionX(this->getPosition().x + nextMoveX);
				verlocity.x = 0;
			}

		}
		//--------------------------------------------------------------------------
		// update object position by verlocity
		//--------------------------------------------------------------------------
		this->setPositionX(verlocity.x + this->getPosition().x);
		this->setPositionY(verlocity.y + this->getPosition().y);

		//////////////////////////////////////////////////////////////////////////////
		//--- apply gravity
		///////////////////////////////////////////////////////////////////////////////
		if (!isOnPlatform)
		{
			verlocity.y -= 0.5f;
		}
		



	}// END IF check object is dynamic object
}

void SimplePhysicObject::setSize(Size _size)
{
	this->size = _size;
	this->centerPoint = Vec2(_size.width / 2, _size.height / 2);
	//--- only auto create polygon for dynamic
	//--- for static object it is optional
	if (this->type == SPOT::SPOT_DYNAMIC){
		//-- auto create object collision points for first
		int pretendX = 4 * 4;
		int pretendY = 6 * 4;
		objectCollisionPoints = {
			Vec2(pretendX, _size.height), Vec2(_size.width - pretendX, _size.height),  // Top of head
			Vec2(pretendX, 0), Vec2(_size.width - pretendX, 0),                        // Feet
			Vec2(0, _size.height - pretendY), Vec2(0, pretendY), // Left arm
			Vec2(_size.width, _size.height - pretendY), Vec2(_size.width, pretendY) // Right arm
		};
	}
}

void SimplePhysicObject::setDebugInfo(Size _size, Color4F _color, Node* parent)
{
	this->onDebugMode = true;
	//--- draw box
	_color.a = 0.7f;
	debugDraw->drawRect(Vec2(0, 0), Vec2(_size.width, _size.height), _color);
	parent->addChild(debugDraw, 1000);
	//--- debug draw center point
	debugDraw->drawPoint(centerPoint, 4.0f, Color4F::RED);
	debugDraw->setPosition(this->getPosition());
}

void SimplePhysicObject::setPosition(Vec2 pos)
{
	this->position = pos;
	//-- debug
	if (onDebugMode){
		debugDraw->setPosition(pos);
	}
}

void SimplePhysicObject::setPositionX(int x)
{
	this->position.x = x;
	//-- debug
	if (onDebugMode){
		debugDraw->setPositionX(x);
	}
}

void SimplePhysicObject::setPositionY(int y)
{
	this->position.y = y;
	//-- debug
	if (onDebugMode){
		debugDraw->setPositionY(y);
	}
}

void SimplePhysicObject::setVelocityX(float x)
{
	this->verlocity.x = x;
}

void SimplePhysicObject::setVelocityY(float y)
{
	this->verlocity.y = y;
}

Vec2 SimplePhysicObject::getPosition()
{
	return this->position;
}



