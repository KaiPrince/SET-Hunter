#include "Command.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "AssetFactory.h"
#include "CollisionResolutionStrategy.h"
#include "Constants.h"



Command::Command()
{
}


Command::~Command()
{
}

void SpawnRocket::Execute()
{
	GameObject* newRocket = new GameObject(_actor->GetXPos(), _actor->GetYPos(), kRocketWidth, kRocketHeight, AssetFactory::GetAsset(DrawableAsset::ROCKET_SPRITE), _world, 0.0f, kRocketYVelocity); //TODO: use prototype pattern.
	newRocket->SetPhysicsComponent(new CollidablePhysicsComponent(newRocket, _world));
	newRocket->GetPhysicsComponent()->SetCollisionStrategy(new RocketCollisionStrategy(newRocket));
	_world->AddGameObject(newRocket);
}

void RemoveGameObjectFromWorldCommand::Execute()
{
	_world->RemoveGameObject(_object);
}
