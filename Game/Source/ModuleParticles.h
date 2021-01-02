#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"

#include "Particle.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

#define MAX_ACTIVE_PARTICLES 8

struct SDL_Texture;
struct Collider;

class ModuleParticles : public Entity
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles();

	//Destructor
	~ModuleParticles();


	bool Start();


	bool PreUpdate() ;


	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	Particle* AddParticle(const Particle& particle, int x, int y, uint delay = 0);

public:
	//Template particle for an explosion
	Particle explosion;

	//Template particle for a laser
	Particle fireBallRight;
	Particle fireBallLeft;
private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

};

#endif // !__MODULEPARTICLES_H__