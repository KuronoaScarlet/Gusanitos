#ifndef __LOGO_H__
#define __LOGO_H__

#include "Module.h"
#include "Animation.h"
#include "Defs.h"


class Logo : public Module
{
public:

	Logo();

	// Destructor
	virtual ~Logo();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider* a, Collider* b);

	bool firstEntry = true;

	

private:

	SDL_Texture* bckground;
	

	bool collision = false;
};


#endif // __SCENE_H__