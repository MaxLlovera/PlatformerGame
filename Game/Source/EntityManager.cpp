#include "EntityManager.h"

#include "Player.h"
#include "Enemy.h"
//#include "Item.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
		case EntityType::PLAYER: ret = new Player();  break;
		case EntityType::ENEMY: ret = new Enemy();  break;
		//case EntityType::ITEM: ret = new Item();  break;
		default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entities.Add(ret);
<<<<<<< HEAD
=======

>>>>>>> 781c6d6d2cec4b864a1563164947fa2745f6f153
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem <Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data == entity)
		{
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}

bool EntityManager::Update(float dt)
{
<<<<<<< HEAD
	for (unsigned int i = 0; i < entities.Count(); i++)
	{
		entities.At(i)->data->Update(dt);
	}
=======
	//if (app->sceneManager->isPaused == false)
	//{
		ListItem<Entity*>* item = entities.start;

		while ((item != nullptr))
		{
			item->data->Update(dt);
			item = item->next;
		}
	//}
>>>>>>> 781c6d6d2cec4b864a1563164947fa2745f6f153

	return true;
}

<<<<<<< HEAD
bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		item->data->PostUpdate();
		item = item->next;
	}

	return true;
}
=======
>>>>>>> 781c6d6d2cec4b864a1563164947fa2745f6f153
