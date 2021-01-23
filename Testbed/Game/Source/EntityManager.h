#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Entity.h"
#include "ModulePhysics.h"

class PhysicsEngine;

class EntityManager : public Module
{
public:
    EntityManager();

    // Destructor
    virtual ~EntityManager()
    {}

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

    void AddEntity(fPoint position, float mass, float weight, float height, Body::Type type);

    void OnCollision(Collider* a, Collider* b);

    PhysicsEngine* integrator = new PhysicsEngine();

    List<Body*> entityList;

private:

    SDL_Texture* texPlayer;

    Body* playerEntity;

    //Fonts
    int scoreFont = -1;
    char scoreText[10] = { "\0" };
};

#endif // _ENTITYMANAGER_H_