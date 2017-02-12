//  Spring structure
//  By D. Buckstein
//	This file was modified by Wednesday-David Hartman, Ty Wood with permission from author.

#ifndef __SPRING_H
#define __SPRING_H

#include "cbmath/cbtkMatrix.h"

class Spring
{
private:
	cbmath::vec3 anchor;
	float lRest, l, dl, vl;
	cbmath::vec3 x, v, xDir, vDir, xPrev;

public:
	Spring(cbmath::vec3 _anchor);
	~Spring() {};

	void updateSpring(const cbmath::vec3 position);
	float getSpringCoefficient(const float mass, const float dt);
	float getDampingCoefficient(const float mass, const float dt);
	//cbmath::vec3 getForceStiffSpring(const float stiff);
	cbmath::vec3 getForceStiffSpringDamped(const float stiff, const float damp);
};

#endif