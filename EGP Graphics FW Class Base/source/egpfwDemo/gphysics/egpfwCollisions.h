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
// inspired by the "slab method": http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
// other optimizations: 
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
// https://tavianator.com/fast-branchless-raybounding-box-intersections/
// https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
struct Ray
{
	// location of ray origin
	Point origin;

	// direction of ray and inverse direction
	Vector direction, directionInv;
};

inline Ray createRay(cbmath::vec3 newOrigin = cbmath::v3zero, cbmath::vec3 newDirection = cbmath::v3zero)
{
	Ray newRay;
	newRay.origin = createPoint(newOrigin);
	newRay.direction = createDirection(newDirection);
	newRay.directionInv = createVector(cbmath::vec3(1.0f / newRay.direction.x, 1.0f / newRay.direction.y, 1.0f / newRay.direction.z));
	return newRay;
}

// use ray structure to produce "line segment"
inline Ray createLine(cbmath::vec3 newOrigin = cbmath::v3zero, cbmath::vec3 newEnd = cbmath::v3zero)
{
	Ray newRay;
	newRay.origin = createPoint(newOrigin);
	newRay.direction = createVector(newEnd - newOrigin);
	newRay.directionInv = createVector(cbmath::vec3(1.0f / newRay.direction.x, 1.0f / newRay.direction.y, 1.0f / newRay.direction.z));
	return newRay;
}


// plane in world, defined implicitly using an existing transform, limits and 
//	distance along the normal to the actual plane itself
struct Plane
{
	cbmath::mat4 localOrientation, localOrientationInverse;
	cbmath::mat4 planeToWorld, worldToPlane;
	cbmath::vec2 maxLimit, minLimit;
	float minDepth;
};

inline Plane createPlane(const cbmath::mat4 localOrientation, const cbmath::vec2 maxLimit, const float planeHeightOnNormal)
{
	Plane newPlane;
	newPlane.localOrientation = localOrientation;
	newPlane.localOrientationInverse = cbmath::transformInverseNoScale(localOrientation);
	newPlane.maxLimit = +maxLimit;
	newPlane.minLimit = -maxLimit;
	newPlane.minDepth = -(planeHeightOnNormal + planeHeightOnNormal);
	return newPlane;
}


//-----------------------------------------------------------------------------
// bounding volume descriptors

// list of things that could be involved in a collision
enum ColliderType
{
	COLLIDER_NONE, 
	COLLIDER_POINT, 
	COLLIDER_RAY, 
	COLLIDER_PLANE, 
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
#define BOX_NUM_CORNERS 8
#define BOX_NUM_PLANES 6
#define BOX_NUM_EDGES 12
struct egpfwBoundingVolumeBox
{
	const egpfwMover *mover;
	const egpfwBox *shapeLimits;	// half dimensions

	// axis-aligned flag
	int isAxisAligned;

	// inverse matrix (box-to-world is stored in mover)
	cbmath::mat4 worldToBox;

	// all corners in local and world space
	Point localCorners[BOX_NUM_CORNERS];
	Point worldCorners[BOX_NUM_CORNERS];

	// all edges in local and world space
	Ray localEdges[BOX_NUM_EDGES];
	Ray worldEdges[BOX_NUM_EDGES];

	// store plane orientations and centers
	Plane planes[BOX_NUM_PLANES];
};


// update volumes
inline void updateBoxVolume(egpfwBoundingVolumeBox *box)
{
	unsigned int i;

	// update world corners
	if (box->isAxisAligned)
	{
		box->worldToBox = box->mover->modelMatrix;
		box->worldToBox.c3.xyz = -box->worldToBox.c3.xyz;
		for (i = 0; i < BOX_NUM_CORNERS; ++i)
			box->worldCorners[i].set(box->mover->modelMatrix.c3.xyz + box->localCorners[i].xyz, 1.0f);
		for (i = 0; i < BOX_NUM_EDGES; ++i)
		{
			box->worldEdges[i] = box->localEdges[i];
			box->worldEdges[i].origin.xyz += box->mover->modelMatrix.c3.xyz;
		}
	}
	else
	{
		box->worldToBox = cbmath::transformInverseNoScale(box->mover->modelMatrix);
		for (i = 0; i < BOX_NUM_CORNERS; ++i)
			box->worldCorners[i] = box->mover->modelMatrix * box->localCorners[i];
		for (i = 0; i < BOX_NUM_EDGES; ++i)
		{
			box->worldEdges[i].origin = box->mover->modelMatrix * box->localEdges[i].origin;
			box->worldEdges[i].direction = box->mover->modelMatrix * box->localEdges[i].direction;
			box->worldEdges[i].directionInv = box->mover->modelMatrix * box->localEdges[i].directionInv;
		}
	}

	// update plane orientations
	for (Plane *pl = box->planes, *end = pl + BOX_NUM_PLANES; pl != end; ++pl)
	{
		pl->worldToPlane = pl->localOrientationInverse * box->worldToBox;
		pl->planeToWorld = box->mover->modelMatrix * pl->localOrientation;
	}
}


// create volumes
inline egpfwBoundingVolumeSphere createSphereVolume(const egpfwMover *mover, const egpfwSphere *sphere)
{
	// just create and return the structure
	egpfwBoundingVolumeSphere ret = { mover, sphere };
	return ret;
}

inline egpfwBoundingVolumeBox createBoxVolume(const egpfwMover *mover, const egpfwBox *box, const int isAxisAligned)
{
	// create structure
	egpfwBoundingVolumeBox ret[1] = { { mover, box, isAxisAligned } };
	const float hw = (float)box->width, hh = (float)box->height, hd = (float)box->depth;

	// set local corners and update
	ret->localCorners[0].set(-hw, -hh, -hd, 1.0f);
	ret->localCorners[1].set(+hw, -hh, -hd, 1.0f);
	ret->localCorners[2].set(-hw, +hh, -hd, 1.0f);
	ret->localCorners[3].set(+hw, +hh, -hd, 1.0f);
	ret->localCorners[4].set(-hw, -hh, +hd, 1.0f);
	ret->localCorners[5].set(+hw, -hh, +hd, 1.0f);
	ret->localCorners[6].set(-hw, +hh, +hd, 1.0f);
	ret->localCorners[7].set(+hw, +hh, +hd, 1.0f);

	// planes: 4x4 transform represents tangent plane
	//	-> relative right is the tangent
	//	-> relative up is the bitangent
	//	-> relative direction is the NORMAL, which we will need later... 
	//	...having a full transformation basis helps with collisions!
	//
	//			  .					  .					
	//			 /|					 /|				
	//			B |				  	B |				N								 . - - -B- - - .	 . - - -B- - - .		
	//		   /| T			  	   /| |		   . - -|-B- - - .	   . - - - - - - .	 |		|	   |	 |		| N	   |		
	//		  .	|/|				  .	| |		  /		|/		/	  /				/	 |		|	   |	 |		|/	   |		
	//		  |	0----- N	 N ---|-1 |		 /		2----- T	 /		3----- T	 |		4----- T	 T -----5	   |		
	//		  |	  .				  |/  .		/			  /		/	   /|	  /		 |	   /	   |	 |			   |		
	//		  |	 /				  T	 /	   . - - - - - - .	   . - - -B-|- - .		 |	  N		   |	 |			   |		
	//		  |	/				  |	/									N			 . - - - - - - .	 . - - - - - - .		
	//		  |/				  |/				
	//		  .					  .							
	//			^^^					^^^
	//			these two look the same
	//			...but they are not
	//		
	// (pro tip: drawing things out like this helps you design algorithms!)
	ret->planes[0] = createPlane(cbmath::mat4(-cbmath::v4z, +cbmath::v4y, +cbmath::v4x, Point(+hw, 0.0f, 0.0f, 1.0f)), cbmath::vec2(hd, hh), hw);	// local +x
	ret->planes[1] = createPlane(cbmath::mat4(+cbmath::v4z, +cbmath::v4y, -cbmath::v4x, Point(-hw, 0.0f, 0.0f, 1.0f)), cbmath::vec2(hd, hh), hw);	// local -x
	ret->planes[2] = createPlane(cbmath::mat4(+cbmath::v4x, -cbmath::v4z, +cbmath::v4y, Point(0.0f, +hh, 0.0f, 1.0f)), cbmath::vec2(hw, hd), hh);	// local +y
	ret->planes[3] = createPlane(cbmath::mat4(+cbmath::v4x, +cbmath::v4z, -cbmath::v4y, Point(0.0f, -hh, 0.0f, 1.0f)), cbmath::vec2(hw, hd), hh);	// local -y
	ret->planes[4] = createPlane(cbmath::mat4(+cbmath::v4x, +cbmath::v4y, +cbmath::v4z, Point(0.0f, 0.0f, +hd, 1.0f)), cbmath::vec2(hw, hh), hd);	// local +z
	ret->planes[5] = createPlane(cbmath::mat4(-cbmath::v4x, +cbmath::v4y, -cbmath::v4z, Point(0.0f, 0.0f, -hd, 1.0f)), cbmath::vec2(hw, hh), hd);	// local -z

	// edges of box
	//	
	//	  2---------3
	//	 /|		   /|
	//	6-+-------7	|
	//	| |		  |	|
	//	| 0-------+-1
	//	|/		  |/
	//	4---------5	
	//	01,13,32,20,45,57,76,64,04,15,37,26
	ret->localEdges[ 0] = createLine(ret->localCorners[0].xyz, ret->localCorners[1].xyz);
	ret->localEdges[ 1] = createLine(ret->localCorners[1].xyz, ret->localCorners[3].xyz);
	ret->localEdges[ 2] = createLine(ret->localCorners[3].xyz, ret->localCorners[2].xyz);
	ret->localEdges[ 3] = createLine(ret->localCorners[2].xyz, ret->localCorners[0].xyz);
	ret->localEdges[ 4] = createLine(ret->localCorners[4].xyz, ret->localCorners[5].xyz);
	ret->localEdges[ 5] = createLine(ret->localCorners[5].xyz, ret->localCorners[7].xyz);
	ret->localEdges[ 6] = createLine(ret->localCorners[7].xyz, ret->localCorners[6].xyz);
	ret->localEdges[ 7] = createLine(ret->localCorners[6].xyz, ret->localCorners[4].xyz);
	ret->localEdges[ 8] = createLine(ret->localCorners[0].xyz, ret->localCorners[4].xyz);
	ret->localEdges[ 9] = createLine(ret->localCorners[1].xyz, ret->localCorners[5].xyz);
	ret->localEdges[10] = createLine(ret->localCorners[3].xyz, ret->localCorners[7].xyz);
	ret->localEdges[11] = createLine(ret->localCorners[2].xyz, ret->localCorners[6].xyz);

	// update transforms
	updateBoxVolume(ret);

	// done
	return *ret;
}


// contact
struct egpfwCollisionContact
{
	// point of contact
	Point location;

	// normal of contact
	Vector normal;

	// interpenetration depth
	float depth;

	// which participant does the contact belong to (0 for A, 1 for B)
	int collider;
};


// generic collision descriptor
struct egpfwCollision
{
	// types of colliders
	ColliderType colliderA, colliderB;

	// participants in collision
	egpfwMover *moverA, *moverB;

	// there can be up to 4 contacts in a 3D collision
	egpfwCollisionContact contact[4];
	unsigned int numContacts;
};
static const egpfwCollision nullCollision = { ColliderType::COLLIDER_NONE, ColliderType::COLLIDER_NONE };

// collision tests
inline int testCollisionSpherePoint(const Point *p, const Point *center, const double radius)
{
	// ****
	return 0;
}

inline int testCollisionSphereRay(const Ray *r, const Point *center, const double radius)
{
	// ****
	return 0;
}

inline int testCollisionSphereEdge(const Ray *r, const Point *center, const double radius)
{
	// ****
	return 0;
}

inline int testCollisionAxisAlignedLocalPoint(const Point *p, const Point *maxLocal, const Point *minLocal)
{
	// ****
	return 0;
}

inline int testCollisionAxisAlignedLocalRay(const Ray *r, const Point *maxLocal, const Point *minLocal)
{
	// 3D slab method
	// some guy optimized the crap outta this... 
	// ...don't get why for loops are used though... no thx
	float t1 = (minLocal->x - r->origin.x) * r->directionInv.x;
	float t2 = (minLocal->x - r->origin.x) * r->directionInv.x;
	float tmin = fmin(t1, t2);
	float tmax = fmax(t1, t2);

	t1 = (minLocal->y - r->origin.y) * r->directionInv.y;
	t2 = (minLocal->y - r->origin.y) * r->directionInv.y;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));

	t1 = (minLocal->z - r->origin.z) * r->directionInv.z;
	t2 = (minLocal->z - r->origin.z) * r->directionInv.z;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));

	return (tmax > fmax(tmin, 0.0f));
}

inline int testCollisionAxisAlignedLocalEdge(const Ray *r, const Point *maxLocal, const Point *minLocal, float *tmax_out, float *tmin_out)
{
	float t1 = (minLocal->x - r->origin.x) * r->directionInv.x;
	float t2 = (minLocal->x - r->origin.x) * r->directionInv.x;
	float tmin = fmin(t1, t2);
	float tmax = fmax(t1, t2);

	t1 = (minLocal->y - r->origin.y) * r->directionInv.y;
	t2 = (minLocal->y - r->origin.y) * r->directionInv.y;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));

	t1 = (minLocal->z - r->origin.z) * r->directionInv.z;
	t2 = (minLocal->z - r->origin.z) * r->directionInv.z;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));

	if (tmax > fmax(tmin, 0.0f) && tmin < fmin(tmax, 1.0f))
	{
		*tmax_out = tmax;
		*tmin_out = tmin;
		return 1;
	}
	return 0;
}


// tests with response
inline egpfwCollision testCollisionSphereSphere(const egpfwBoundingVolumeSphere *sphereVolA, const egpfwBoundingVolumeSphere *sphereVolB)
{
	egpfwCollision result = nullCollision;
	if (sphereVolA && sphereVolB)
	{
		// ****
	}
	return result;
}

inline egpfwCollision testCollisionSphereBox(const egpfwBoundingVolumeSphere *sphereVol, const egpfwBoundingVolumeBox *boxVol)
{
	egpfwCollision result = nullCollision;
	if (sphereVol && boxVol)
	{
		// easier if the sphere is strictly relative to the box
		const Point *maxLocal = boxVol->localCorners + 7, *minLocal = boxVol->localCorners;
		const Point localSphereCenter = boxVol->worldToBox * sphereVol->mover->modelMatrix.c3;
		Point closestPoint[1] = { localSphereCenter }, localCenter[1] = { localSphereCenter };

		// ****
	}
	return result;
}


inline int testCollisionAxisAlignedAxisAligned(const egpfwBoundingVolumeBox *axisAlignedA, const egpfwBoundingVolumeBox *axisAlignedB)
{
	// ****
	return 0;
}

inline int testCollisionAxisAlignedBox(const egpfwBoundingVolumeBox *axisAligned, const egpfwBoundingVolumeBox *boxVol)
{
	// ****
	return 0;
}

inline int testCollisionBoxAxisAligned(const egpfwBoundingVolumeBox *boxVol, const egpfwBoundingVolumeBox *axisAligned)
{
	// ****
	return 0;
}


inline int testPlanesVsCorners(const Plane *localPlane, const Point *otherCorner, Point *otherCornerLocalized, egpfwCollisionContact *contactBase, int contactOwner)
{
	int numContacts = 0;
    
    float smallestDepth;
    
    const Plane *iteratorStart = localPlane,
                *iteratorEnd = iteratorStart + BOX_NUM_PLANES,
                *hitPlane;
    
    Point *cornerPoint, *hitCorner = nullptr;

	// ****
    
    //iterate through each point on the other box
    for(const Point *cornerEnd = otherCorner + BOX_NUM_CORNERS; otherCorner != cornerEnd; ++otherCorner)
    {
        smallestDepth = -1000000.0f;
        hitPlane = 0;
        
        //iterate through each plane on this box
        for(localPlane = iteratorStart, cornerPoint = otherCornerLocalized; localPlane != iteratorEnd; ++localPlane, ++cornerPoint)
        {
            //bring corner into plane's space
            *cornerPoint = localPlane->worldToPlane * *otherCorner;
            
            //point vs plane test
            //smallest intersection is kept as potential contact
            if(cornerPoint->z > smallestDepth && cornerPoint->z <= 0.0f && cornerPoint->z >= localPlane->minDepth &&
               cornerPoint->x <= localPlane->maxLimit.x && cornerPoint->x >= localPlane->minLimit.x &&
               cornerPoint->y <= localPlane->maxLimit.y && cornerPoint->y >= localPlane->minLimit.y)
            {
                hitPlane = localPlane;
                hitCorner = cornerPoint;
                smallestDepth = cornerPoint->z;
            }
        }
        
        //if any contact, store in result
        if(hitPlane)
        {
            contactBase->location = hitPlane->planeToWorld * *hitCorner;
            contactBase->normal   = hitPlane->planeToWorld.c2;
            contactBase->depth    = -smallestDepth;
            contactBase->collider = contactOwner;
            
            ++contactBase;
            ++numContacts;
        }
    }
    

	return numContacts;
}

inline int testPlanesVsEdges(const Plane *localPlane, const Ray *otherEdge, Point *hitPointsLocalized, egpfwCollisionContact *contactBase, int contactOwner)
{
	int numContacts = 0;

	// ****

	return numContacts;
}


// *THIS FUNCTION IS COMPLETE!
// however, if the above tests are completed, this function contains a 
//	version of the general algorithm that would result in "false positives"
//	(block comment at the bottom of the function)
inline egpfwCollision testCollisionBoxBox(const egpfwBoundingVolumeBox *boxVolA, const egpfwBoundingVolumeBox *boxVolB)
{
	egpfwCollision result = nullCollision;
	if (boxVolA && boxVolB)
	{
		// below is a solution for just checking IF there was a collision
		// here is a general solution for actually getting contact info
		// start by switching coordinate spaces
		const egpfwBoundingVolumeBox *local, *other;
		int contactOwner;
		Point localizedPoint[6];

		// first check all the points of the first until at least one contact is generated
		// move into box's space, then move into plane's space
		// check if transformed point is both within the bounds of the plane, 
		//	but not past the opposite plane (test Z first!)
	//	if (!result.numContacts)	// always true
		{
			local = boxVolA;
			other = boxVolB;
			contactOwner = 1;
			result.numContacts = testPlanesVsCorners(local->planes, other->worldCorners, localizedPoint, result.contact, contactOwner);
		}

		// failed; check all the points of the other
		if (!result.numContacts)
		{
			local = boxVolB;
			other = boxVolA;
			contactOwner = 0;
			result.numContacts = testPlanesVsCorners(local->planes, other->worldCorners, localizedPoint, result.contact, contactOwner);
		}

		// failed; check edges of first
		if (!result.numContacts)
		{
			local = boxVolA;
			other = boxVolB;
			contactOwner = 1;
			result.numContacts = testPlanesVsEdges(local->planes, other->worldEdges, localizedPoint, result.contact, contactOwner);
		}

		// failed; check edges of other
		if (!result.numContacts)
		{
			local = boxVolB;
			other = boxVolA;
			contactOwner = 0;
			result.numContacts = testPlanesVsEdges(local->planes, other->worldEdges, localizedPoint, result.contact, contactOwner);
		}

		// check if successful by the end of the algorithm
		if (result.numContacts)
		{
			result.moverA = (egpfwMover *)boxVolA->mover;
			result.moverB = (egpfwMover *)boxVolB->mover;
			result.colliderA = boxVolA->isAxisAligned ? ColliderType::COLLIDER_BOX_AXIS_ALIGNED : ColliderType::COLLIDER_BOX_OBJECT;
			result.colliderB = boxVolB->isAxisAligned ? ColliderType::COLLIDER_BOX_AXIS_ALIGNED : ColliderType::COLLIDER_BOX_OBJECT;
		}

/*
		// different algorithm if they are not axis-aligned
		// handle in order of fastest to slowest
		// fastest: both axis-aligned
		if (boxVolA->isAxisAligned && boxVolB->isAxisAligned)
		{
			if (testCollisionAxisAlignedAxisAligned(boxVolA, boxVolB))
			{
				result.colliderA = result.colliderB = ColliderType::COLLIDER_BOX_AXIS_ALIGNED;
			}
		}
		else if (boxVolA->isAxisAligned)
		{
			if (testCollisionAxisAlignedBox(boxVolA, boxVolB) &&
				testCollisionBoxAxisAligned(boxVolB, boxVolA))
			{
				result.colliderA = ColliderType::COLLIDER_BOX_AXIS_ALIGNED;
				result.colliderB = ColliderType::COLLIDER_BOX_OBJECT;
			}
		}
		else if (boxVolB->isAxisAligned)
		{
			if (testCollisionAxisAlignedBox(boxVolB, boxVolA) &&
				testCollisionBoxAxisAligned(boxVolA, boxVolB))
			{
				result.colliderA = ColliderType::COLLIDER_BOX_OBJECT;
				result.colliderB = ColliderType::COLLIDER_BOX_AXIS_ALIGNED;
			}
		}
		else
		{
			if (testCollisionBoxAxisAligned(boxVolA, boxVolB) &&
				testCollisionBoxAxisAligned(boxVolB, boxVolA))
			{
				result.colliderA = result.colliderB = ColliderType::COLLIDER_BOX_OBJECT;
			}
		}
*/
	}
	return result;
}


//-----------------------------------------------------------------------------


#endif	// __EGPFW_COLLISIONS_H
