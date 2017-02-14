// Collision generator functions
// By D. Buckstein
// Modified by: 

#ifndef __EGPFW_COLLISIONS_H
#define __EGPFW_COLLISIONS_H


#include "egpfwMover.h"
#include "egpfwShapes.h"


//-----------------------------------------------------------------------------
// basic types

// point/vector
typedef cbmath::vec4 Point, Vector;
inline Point createPoint(cbmath::vec3 newPosition = cbmath::v3zero)
{
	return Point(newPosition, 1.0f);
}

inline Vector createVector(cbmath::vec3 newVector = cbmath::v3zero)
{
	return Vector(newVector);
}

inline Vector createDirection(cbmath::vec3 newVector = cbmath::v3zero)
{
	return Vector(cbmath::normalize(newVector));
}

// ray in world
struct Ray
{
	Point origin;
	Vector direction;
};

inline Ray createRay(cbmath::vec3 newOrigin = cbmath::v3zero, cbmath::vec3 newDirection = cbmath::v3zero)
{
	Ray newRay;
	newRay.origin = createPoint(newOrigin);
	newRay.direction = createDirection(newDirection);
	return newRay;
}


//-----------------------------------------------------------------------------
// bounding volume descriptors

// list of things that could be involved in a collision
enum ColliderType
{
	COLLIDER_NONE, 
	COLLIDER_POINT, 
	COLLIDER_RAY, 
	COLLIDER_SPHERE, 
	COLLIDER_CIRCLE, 
	COLLIDER_BOX_AXIS_ALIGNED, 
	COLLIDER_BOX_OBJECT, 
};


// circle/sphere
struct egpfwBoundingVolumeSphere
{
	const egpfwMover *mover;		// holds transform
	const egpfwSphere *shape;		// holds radius
};

// plane/box
struct egpfwBoundingVolumeBox
{
	const egpfwMover *mover;
	const egpfwBox *shapeLimits;	// half dimensions

	// axis-aligned flag
	int isAxisAligned;

	// all corners in local and world space
	Point localCorners[8];
	Point worldCorners[8];
	unsigned int numCorners;		// 4 if 2D, 8 if 3D
};


// update volumes
inline void updateBoxVolume(egpfwBoundingVolumeBox *box)
{
	// update world corners
	for (unsigned int i = 0; i < box->numCorners; ++i)
		box->worldCorners[i] = box->mover->modelMatrix * box->localCorners[i];
}


// create volumes
inline egpfwBoundingVolumeSphere createSphereVolume(const egpfwMover *mover, const egpfwSphere *sphere)
{
	// just create and return the structure
	egpfwBoundingVolumeSphere ret = { mover, sphere };
	return ret;
}

inline egpfwBoundingVolumeBox createBoxVolume(const egpfwMover *mover, const egpfwBox *box, const int isAxisAligned, const int is3D)
{
	// create structure
	egpfwBoundingVolumeBox ret = { mover, box, isAxisAligned };
	const float hw = (float)box->width, hh = (float)box->height, hd = (float)box->depth;

	// set local corners and update
	ret.localCorners[0].set(-hw, -hh, -hd, 1.0f);
	ret.localCorners[1].set(+hw, -hh, -hd, 1.0f);
	ret.localCorners[2].set(-hw, +hh, -hd, 1.0f);
	ret.localCorners[3].set(+hw, +hh, -hd, 1.0f);
	ret.localCorners[4].set(-hw, -hh, +hd, 1.0f);
	ret.localCorners[5].set(+hw, -hh, +hd, 1.0f);
	ret.localCorners[6].set(-hw, +hh, +hd, 1.0f);
	ret.localCorners[7].set(+hw, +hh, +hd, 1.0f);
	ret.numCorners = is3D ? 8 : 4;
	updateBoxVolume(&ret);

	// done
	return ret;
}


// generic collision descriptor
struct egpfwCollision
{
	ColliderType colliderA, colliderB;
};
static const egpfwCollision nullCollision = { ColliderType::COLLIDER_NONE, ColliderType::COLLIDER_NONE };

// collision tests
inline egpfwCollision testCollisionSpherePoint(const egpfwBoundingVolumeSphere *sphereVolume, const Point *p)
{
	egpfwCollision result = nullCollision;
	if (sphereVolume && p)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionSphereRay(const egpfwBoundingVolumeSphere *sphereVolume, const Ray *r)
{
	egpfwCollision result = nullCollision;
	if (sphereVolume && r)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionBoxPoint(const egpfwBoundingVolumeBox *boxVolume, const Point *p)
{
	egpfwCollision result = nullCollision;
	if (boxVolume && p)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionBoxRay(const egpfwBoundingVolumeBox *boxVolume, const Ray *r)
{
	egpfwCollision result = nullCollision;
	if (boxVolume && r)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionSphereSphere(const egpfwBoundingVolumeSphere *sphereVolumeA, const egpfwBoundingVolumeSphere *sphereVolumeB)
{
	egpfwCollision result = nullCollision;
	if (sphereVolumeA && sphereVolumeB)
	{
        float distance = cbmath::lengthSq(sphereVolumeA->mover->position - sphereVolumeB->mover->position);
        
        float radii = sphereVolumeA->shape->radius + sphereVolumeB->shape->radius;
        
        if(distance < radii*radii)
        {
            result = { COLLIDER_SPHERE, COLLIDER_SPHERE };
        }
	}
	return result;
}

inline egpfwCollision testCollisionSphereBox(const egpfwBoundingVolumeSphere *sphereVolume, const egpfwBoundingVolumeBox *boxVolume)
{
	egpfwCollision result = nullCollision;
	if (sphereVolume && boxVolume)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionBoxBox(const egpfwBoundingVolumeBox *boxVolumeA, const egpfwBoundingVolumeBox *boxVolumeB)
{
	egpfwCollision result = nullCollision;
	if (boxVolumeA && boxVolumeB)
	{
		if(boxVolumeA->isAxisAligned && boxVolumeB->isAxisAligned)
        {
            const Point maxA = boxVolumeA->worldCorners[7];
            const Point minA = boxVolumeA->worldCorners[0];
            const Point maxB = boxVolumeB->worldCorners[7];
            const Point minB = boxVolumeB->worldCorners[0];

            if(maxA.x >= minB.x && maxB.x >= minA.x &&
               maxA.y >= minB.y && maxB.y >= minA.y &&
               maxA.z >= minB.z && maxB.z >= minA.z)
            {
                result = { COLLIDER_BOX_AXIS_ALIGNED, COLLIDER_BOX_AXIS_ALIGNED };
            }
        }
        else
        {
            // corner index, inverse matric, transformed corner, keep track of max and min
            unsigned int i;
            cbmath::mat4 inverseMatrix;
            Point transformedCorner;
            Point minOther, maxOther, minLocal, maxLocal;
            const egpfwBoundingVolumeBox *local, *other;
            
            // first test: box B into box A space
            
            local = boxVolumeA; other = boxVolumeB;
            
            maxLocal = local->localCorners[7]; minLocal = local->localCorners[0];
            maxOther.set(-100000.0f).w = 1.0f; minOther.set(100000.0f).w = 1.0f;
            
            inverseMatrix = cbmath::transformInverseNoScale(local->mover->modelMatrix);
            for(i = 0; i < other->numCorners; i++)
            {
                transformedCorner = inverseMatrix * other->worldCorners[i];
                if(transformedCorner.x < minOther.x) minOther.x = transformedCorner.x;
                if(transformedCorner.y < minOther.y) minOther.y = transformedCorner.y;
                if(transformedCorner.z < minOther.z) minOther.z = transformedCorner.z;
                
                if(transformedCorner.x > maxOther.x) maxOther.x = transformedCorner.x;
                if(transformedCorner.y > maxOther.y) maxOther.y = transformedCorner.y;
                if(transformedCorner.z > maxOther.z) maxOther.z = transformedCorner.z;
            }
            
            if(maxLocal.x >= minOther.x && maxOther.x >= minLocal.x &&
               maxLocal.y >= minOther.y && maxOther.y >= minLocal.y &&
               maxLocal.z >= minOther.z && maxOther.z >= minLocal.z)
            {
                local = boxVolumeB; other = boxVolumeA;
                
                maxLocal = local->localCorners[7]; minLocal = local->localCorners[0];
                maxOther.set(-100000.0f).w = 1.0f; minOther.set(100000.0f).w = 1.0f;
                
                inverseMatrix = cbmath::transformInverseNoScale(local->mover->modelMatrix);
                for(i = 0; i < other->numCorners; i++)
                {
                    transformedCorner = inverseMatrix * other->worldCorners[i];
                    if(transformedCorner.x < minOther.x) minOther.x = transformedCorner.x;
                    if(transformedCorner.y < minOther.y) minOther.y = transformedCorner.y;
                    if(transformedCorner.z < minOther.z) minOther.z = transformedCorner.z;
                    
                    if(transformedCorner.x > maxOther.x) maxOther.x = transformedCorner.x;
                    if(transformedCorner.y > maxOther.y) maxOther.y = transformedCorner.y;
                    if(transformedCorner.z > maxOther.z) maxOther.z = transformedCorner.z;
                }
                
                if(maxLocal.x >= minOther.x && maxOther.x >= minLocal.x &&
                   maxLocal.y >= minOther.y && maxOther.y >= minLocal.y &&
                   maxLocal.z >= minOther.z && maxOther.z >= minLocal.z)
                {
                    result.colliderA = boxVolumeA->isAxisAligned ? COLLIDER_BOX_AXIS_ALIGNED : COLLIDER_BOX_OBJECT;
                    result.colliderB = boxVolumeB->isAxisAligned ? COLLIDER_BOX_AXIS_ALIGNED : COLLIDER_BOX_OBJECT;
                }
            }
        }
	}
	return result;
}


//-----------------------------------------------------------------------------


#endif	// __EGPFW_COLLISIONS_H
