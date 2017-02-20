//  David Hartman 0923439
//  Ty Wood       0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.05
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.





#ifndef Eases_h
#define Eases_h





#include <stdio.h>
#include "cbmath/cbtkMatrix.h"
#include <cmath>





// a namespace for different types of eases
// ported from Ty's personal Swift library
// http://easings.net
namespace TimingFunctions
{
    inline float Linear(float t)
    {
        return t;
    }
    
    inline float QuadraticEaseIn(float t)
    {
        return t * t;
    }
    
    inline float QuadraticEaseOut(float t)
    {
        return t * (2.0 - t);
    }
    
    inline float QuadraticEaseInOut(float t)
    {
        if (t < 0.5)
        {
            return 2.0 * t * t;
        }
        else
        {
            float f = t - 1.0;
            return 1.0 - 2.0 * f * f;
        }
    }
    
    inline float CubicEaseIn(float t)
    {
        return t * t * t;
    }
    
    inline float CubicEaseOut(float t)
    {
        float f = t - 1.0;
        return 1.0 + f * f * f;
    }
    
    inline float CubicEaseInOut(float t)
    {
        if (t < 0.5)
        {
            return 4.0 * t * t * t;
        }
        else
        {
            float f = t - 1.0;
            return 1.0 + 4.0 * f * f * f;
        }
    }
    
    inline float QuarticEaseIn(float t)
    {
        return t * t * t * t;
    }
    
    inline float QuarticEaseOut(float t)
    {
        float f = t - 1.0;
        return 1.0 - f * f * f * f;
    }
    
    inline float QuarticEaseInOut(float t)
    {
        if (t < 0.5)
        {
            return 8.0 * t * t * t * t;
        }
        else
        {
            float f = t - 1.0;
            return 1.0 - 8.0 * f * f * f * f;
        }
    }
    
    inline float QuinticEaseIn(float t)
    {
        return t * t * t * t * t;
    }
    
    inline float QuinticEaseOut(float t)
    {
        float f = t - 1.0;
        return 1.0 + f * f * f * f * f;
    }
    
    inline float QuinticEaseInOut(float t)
    {
        if (t < 0.5)
        {
            return 16.0 * t * t * t * t * t;
        }
        else
        {
            float f = t - 1.0;
            return 1.0 + 16.0 * f * f * f * f * f;
        }
    }
    
    inline float SineEaseIn(float t)
    {
        return sin((t - 1.0) * Pi()/2) + 1.0;
    }
    
    inline float SineEaseOut(float t)
    {
        return sin(t * Pi()/2);
    }
    
    inline float SineEaseInOut(float t)
    {
        return 0.5 * (1.0 - cos(t * Pi()));
    }
    
    inline float CircularEaseIn(float t)
    {
        return 1.0 - sqrt(1.0 - t * t);
    }
    
    inline float CircularEaseOut(float t)
    {
        return sqrt((2.0 - t) * t);
    }
    
    inline float CircularEaseInOut(float t)
    {
        if (t < 0.5)
        {
            return 0.5 * (1.0 - sqrt(1.0 - 4.0 * t * t));
        }
        else
        {
            return 0.5 * sqrt(-4.0 * t * t + 8.0 * t - 3.0) + 0.5;
        }
    }
    
    inline float ExponentialEaseIn(float t)
    {
        return (t == 0.0) ? t : pow(2.0, 10.0 * (t - 1.0));
    }
    
    inline float ExponentialEaseOut(float t)
    {
        return (t == 1.0) ? t : 1.0 - pow(2.0, -10.0 * t);
    }
    
    inline float ExponentialEaseInOut(float t)
    {
        if (t == 0.0 || t == 1.0)
        {
            return t;
        }
        else if (t < 0.5)
        {
            return 0.5 * pow(2.0, 20.0 * t - 10.0);
        }
        else
        {
            return 1.0 - 0.5 * pow(2.0, -20.0 * t + 10.0);
        }
    }
    
    inline float Smoothstep(float t)
    {
        return t * t * (3 - 2 * t);
    }
    
    inline float Smootherstep(float t)
    {
        return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
    }
}





// namespace for lerping data
// written by: Ty
namespace Eases
{
    // function that returns a value between start and end at the given percent of time, using the passed in easetype (function pointer)
    template <typename T>
    T lerp(T start, T end, float percent, float (*f)(float))
    {
        return (start + (*f)(percent)*(end - start));
    }
}
#endif /* Eases_h */
