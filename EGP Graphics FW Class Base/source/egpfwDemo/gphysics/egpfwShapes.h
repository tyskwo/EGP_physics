// Simple shape descriptors and utilities
// By D. Buckstein
// Modified by: 

#ifndef __EGPFW_SHAPES_H
#define __EGPFW_SHAPES_H


#define pi_egp		3.1415926535897932384626433832795
#define pi_egpf		3.1415926535897932384626433832795f
#define halfpi_egp	1.5707963267948966192313216916398
#define halfpi_egpf	1.5707963267948966192313216916398f
#define twopi_egp	6.283185307179586476925286766559
#define twopi_egpf	6.283185307179586476925286766559f
#define fourpi_egp	12.566370614359172953850573533118
#define fourpi_egpf	12.566370614359172953850573533118f
#define fourthirdspi_egp	4.1887902047863909846168578443727
#define fourthirdspi_egpf	4.1887902047863909846168578443727f


#ifdef __cplusplus
extern"C"
{
#endif	// __cplusplus


	struct egpfwCircle
	{
		double radius;
	};

	struct egpfwSphere
	{
		double radius;
	};

	struct egpfwPlane
	{
		double width, height;
	};

	struct egpfwBox
	{
		double width, height, depth;
	};


#ifndef __cplusplus
	typedef struct egpfwCircle	egpfwCircle;
	typedef struct egpfwSphere	egpfwSphere;
	typedef struct egpfwPlane	egpfwPlane;
	typedef struct egpfwBox		egpfwBox;
#endif	// !__cplusplus


	// calculate areas and volumes of shapes
	inline double getCircleCircumference(const egpfwCircle *circle)
	{
		return (twopi_egp * circle->radius);
	}

	inline double getCircleArea(const egpfwCircle *circle)
	{
		return (pi_egp * circle->radius*circle->radius);
	}

	inline double getSphereCrossSectionArea(const egpfwSphere *sphere)
	{
		return (pi_egp * sphere->radius*sphere->radius);
	}

	inline double getSphereSurfaceArea(const egpfwSphere *sphere)
	{
		return (fourpi_egp * sphere->radius*sphere->radius);
	}

	inline double getSphereVolume(const egpfwSphere *sphere)
	{
		return (fourthirdspi_egp * sphere->radius*sphere->radius*sphere->radius);
	}

	inline double getPlanePerimeter(const egpfwPlane *plane)
	{
		return (plane->width + plane->height)*2.0;
	}

	inline double getPlaneArea(const egpfwPlane *plane)
	{
		return (plane->width * plane->height);
	}

	inline double getBoxSurfaceArea(const egpfwBox *box) 
	{
		return (box->width*box->height + box->width*box->depth + box->height*box->depth)*2.0;
	}

	inline double getBoxVolume(const egpfwBox *box)
	{
		return (box->width * box->height * box->depth);
	}


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// __EGPFW_SHAPES_H