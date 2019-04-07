#include "CollisionResolutionStrategy.h"
#include "GameObject.h"






CollisionResolutionStrategy::CollisionResolutionStrategy(GameObject* obj)
{
	this->_obj = obj;
}

CollisionResolutionStrategy::~CollisionResolutionStrategy()
{
}

void PlayerCollisionStrategy::CollideWith(GameObject* obj)
{

}

NullCollisionStrategy::NullCollisionStrategy() : CollisionResolutionStrategy(new NullGameObject())
{
}
