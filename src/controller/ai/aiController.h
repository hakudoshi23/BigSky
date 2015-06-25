#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H
  
#include "../../framework.h"
#include "../../includes.h"

#include "../controller.h"
#include "../../world/world.h"
#include "../../entity/entityPlane.h"

enum {AI_STATE_PATROL, AI_STATE_ATTACK};

class AIController : public Controller
{
public:
	int entity_id;
	float shootCD, lastShoot;
	
	int state, current_patrol_index;
	std::vector<Vector3> patroll_points;
	float detect_radius;

	AIController();
	~AIController();
	
	void init();
	void update(double delta);

	float chase(Vector3 target){
		EntityPlane* plane = (EntityPlane*) World::getInstance()->findByID(this->entity_id);

		Vector3 pos = Vector3(plane->model.m[12], plane->model.m[13], plane->model.m[14]);
		Vector3 left = plane->model.rotateVector(Vector3(1, 0, 0));
		Vector3 up = plane->model.rotateVector(Vector3(0, 1, 0));

		Vector3 toPoint = target - pos;
		float dst = toPoint.length();
		toPoint.normalize();

		float _left = left.dot(toPoint);
		float _up = up.dot(toPoint);

		if(abs(_left) > 0.001) plane->yaw(-_left * 2);
		if(abs(_up) > 0.001) plane->pitch(_up * 2);

		return dst;
	}
};

#endif