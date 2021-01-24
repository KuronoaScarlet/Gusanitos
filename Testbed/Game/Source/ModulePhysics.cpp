#include "ModulePhysics.h"

#include "App.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

//Llamamos a la funcion de IntegratorPhysics que hemos creado. En ella pasamos la posicion del cuerpo, la masa, el centro, el vector de velocidad, 
//la superficie, la coeficiente de sustentacion, la velocidad relativa, el volumen de agua desplazado(en caso de que haya agua) y un bool para que se active la fuerza de Bouyancy
//Un ejemplo de donde puedes ver esto aplicado es en el Player.cpp, en el Update.
fPoint PhysicsEngine::IntegratePhysics(fPoint position, float mass, fPoint massCentre, fPoint dirVelo, float surface, float cd, float velRelative, float volume, bool inWater)
{
	//Dentro de la funcion, nuestro objetivo es calcular todas las fuerzas que se aplican en un cuerpo. La fuerza de gravedad y Aerodinamica siempre se calculan
	//en el cuerpo, en cambio, la fuerza de bouyancy solo la calcularemos en caso de que este en contacto con el agua.
	ForceGrav(position, mass, massCentre);
	Aerodeynamics(dirVelo, surface, cd, velRelative);
	if (inWater) Bouyancy(position, mass, volume);
	else Bouyancy({ 0,0 }, 0, 0);
	return Acceleration(mass);
}

void PhysicsEngine::ForceGrav(fPoint position, float mass, fPoint massCentre)
{
	// Cálculo de la fuerza gravitatoria de nuestro mundo. El reescalado de las unidades hace que la fuerza G dentro del juego sea 8 veces la real, pero funciona correctamente.
	radius = world->centre.x;
	sqRadius = (pow((radius), 2));
	gravityForce.x = 0;
	gravityForce.y = G * (world->mass / sqRadius);
}
fPoint PhysicsEngine::Acceleration(float mass)
{
	//Para que el jugador se mueva, usaremos la aceleración resulatante de todas sus fuerzas. Para sacarla usaremos la formula de F = ma ----> a = f/m.
	//df = F
	df.x = gravityForce.x + aerodinamicForce.x + hydrodinamicForce.x + normalForce.x;
	df.y = gravityForce.y + aerodinamicForce.y + hydrodinamicForce.y + normalForce.y;
	return { df.x / mass, df.y / mass };
}
void PhysicsEngine::Aerodeynamics(fPoint dirVelo, float surface, float cd, float velRelative)
{
	//En caso de que no se use la fuerza, usaremos este if que hace que en caso de que haya zero no dé indterminación.
	if (dirVelo != fPoint{ 0,0 })
	{
		dirVelo.x = dirVelo.x / (sqrt((dirVelo.x * dirVelo.x) + (dirVelo.y * dirVelo.y)));
		dirVelo.y = dirVelo.y / (sqrt((dirVelo.x * dirVelo.x) + (dirVelo.y * dirVelo.y)));
	}
	float moduleAF;
	moduleAF = 0.5 * AIR_DENSITY * pow(velRelative, 2) * surface * cd;
	aerodinamicForce.x = moduleAF * dirVelo.x;
	aerodinamicForce.y = -moduleAF * dirVelo.y;
}
void PhysicsEngine::Bouyancy(fPoint position, float mass, float volume)
{
	//Al hacer contacto con el agua, se calulara solamente la fuerza de bouyancy en ya que queremos que flote.
	hydrodinamicForce.x = 0;
	hydrodinamicForce.y = (WATER_DENSITY * gravityForce.y * volume - mass * gravityForce.y)/150;
}

fPoint PhysicsEngine::Integrator(float dt, fPoint* position, fPoint* velocity, fPoint* acceleration)
{
	//Una vez que, en el update del el cuerpo que queramos mover, se haya calculado la aceleración, se llamara a la funcion de Integrator. En esta funcion se pasa el timepo entre 
	//frame y frame, su posicion, su velocidad y la aceleración.
	//Luego se aplican las formulas de Velocity-Verlet para aplicar el cambio de posicion al cuerpo.
	position->x += velocity->x * dt + 0.5f * (acceleration->x * dt * dt);
	position->y += velocity->y * dt + 0.5f * (acceleration->y * dt * dt);

	velocity->x += acceleration->x * dt;
	velocity->y += acceleration->y * dt;

	return (*position);
}

//En caso de que queramos que el player se mueva manualmente, crearemos las funcion de AddMomentum. 
//Esta funcion se usara para aplicar un momento a un cuerpo. Luego con este momento, se calculara la velocidad y haran calculos para cambiar la posición.
fPoint PhysicsEngine::AddMomentum(fPoint momentum, float mass, fPoint velocity)
{
	fPoint vel;
	if (momentum.x != 0) vel.x = momentum.x / mass;
	if (momentum.y != 0) vel.y = momentum.y / mass;

	return vel;
}

//Funcion para simplificar el calculo del modulo entre dos vectores
float PhysicsEngine::Normalize(fPoint p0, fPoint p1)
{
	fPoint v;
	v.x = p0.x - p1.x;
	v.y = p0.y - p1.y;
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}