#include "CollisionResolutionStrategy.h"
#include "GameObject.h"
#include "GameObjectState.h"
#include "Command.h"
#include "GameWorld.h"

NullCollisionStrategy* NullCollisionStrategy::_singleton;

void PlayerCollisionStrategy::visit(DeathTouchCollisionStrategy* component)
{
	//Die.
	if (_obj->GetState()->GetType() == GameObjectState::ALIVE_STATE)
	{
		_obj->UpdateState(new DeadState(_obj));
	}
}

NullCollisionStrategy::NullCollisionStrategy() : CollisionResolutionStrategy(nullptr)
{
}

void RocketCollisionStrategy::visit(PlayerCollisionStrategy* component)
{
	this->_obj->GetGameWorld()->QueueCommand(new RemoveGameObjectFromWorldCommand(this->_obj, this->_obj->GetGameWorld()));
}

void CollisionResolutionStrategy::CollideWith(GameObject* collidedObj)
{
	collidedObj->GetPhysicsComponent()->GetCollisionStrategy()->accept(this);
}
