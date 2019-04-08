#include "CollisionResolutionStrategy.h"
#include "GameObject.h"
#include "GameObjectState.h"

NullCollisionStrategy* NullCollisionStrategy::_singleton;

void PlayerCollisionStrategy::CollideWith(GameObject* collidedObj)
{
	collidedObj->GetPhysicsComponent()->GetCollisionStrategy()->accept(this);
}

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

void DeathTouchCollisionStrategy::CollideWith(GameObject* collidedObj)
{
	collidedObj->GetPhysicsComponent()->GetCollisionStrategy()->accept(this);
}
