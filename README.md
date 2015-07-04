# cocos2dx_simplephysic
after a time using cocos2dx i think if want to make a 2d platform game i don't really need some engine like box2D or chipmunk
so after looking for some source. i created this, pretty simple to use and easy to extend.

USING: 

1, create world:

```c++
//--- create a note to contain debug node
auto debugDraw = Node::create();
debugDraw->setAnchorPoint({ 0, 0 });
this->addChild(debugDraw, 1000);
//--- create test physic world
testPhysicWorld = SimplePhysicWorld::create();
testPhysicWorld->setGravity(Vec2(0, -9.8f));
testPhysicWorld->setDebugDisplay(debugDraw);
```
2, create a object 
```c++
auto box = SimplePhysicObject::create(SimplePhysicObject::SPOT_STATIC);
box->setSize(Size(64, 64));
box->setPosition(Vec2(newX * 64, newY * 64));
testPhysicWorld->addChild(box);
