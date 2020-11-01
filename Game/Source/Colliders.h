#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"

#define MAX_COLLIDERS 200




class Colliders : public Module
{
public:
	Colliders();
	virtual ~Colliders();
	
	enum Type
	{
		NONE = -1,
		GROUND,
		PLAYER,
		
		MAX
	};


	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;


	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


private:
	SDL_Rect rect;
	Type type;
};





#endif // !__COLLIDER_H__