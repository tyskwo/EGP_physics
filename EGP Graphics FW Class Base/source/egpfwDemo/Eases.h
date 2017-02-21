//  Wednesday-David Hartman 0923439
//  Ty Wood					0901793
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
        return t * (2.0f - t);
    }
    
    inline float QuadraticEaseInOut(float t)
    {
        if (t < 0.5f)
        {
            return 2.0f * t * t;
        }
        else
        {
            float f = t - 1.0f;
            return 1.0f - 2.0f * f * f;
        }
    }
    
    inline float CubicEaseIn(float t)
    {
        return t * t * t;
    }
    
    inline float CubicEaseOut(float t)
    {
        float f = t - 1.0f;
        return 1.0f + f * f * f;
    }
    
    inline float CubicEaseInOut(float t)
    {
        if (t < 0.5f)
        {
            return 4.0f * t * t * t;
        }
        else
        {
            float f = t - 1.0f;
            return 1.0f + 4.0f * f * f * f;
        }
    }
    
    inline float QuarticEaseIn(float t)
    {
        return t * t * t * t;
    }
    
    inline float QuarticEaseOut(float t)
    {
        float f = t - 1.0f;
        return 1.0f - f * f * f * f;
    }
    
    inline float QuarticEaseInOut(float t)
    {
        if (t < 0.5f)
        {
            return 8.0f * t * t * t * t;
        }
        else
        {
            float f = t - 1.0f;
            return 1.0f - 8.0f * f * f * f * f;
        }
    }
    
    inline float QuinticEaseIn(float t)
    {
        return t * t * t * t * t;
    }
    
    inline float QuinticEaseOut(float t)
    {
        float f = t - 1.0f;
        return 1.0f + f * f * f * f * f;
    }
    
    inline float QuinticEaseInOut(float t)
    {
        if (t < 0.5f)
        {
            return 16.0f * t * t * t * t * t;
        }
        else
        {
            float f = t - 1.0f;
            return 1.0f + 16.0f * f * f * f * f * f;
        }
    }
    
    inline float SineEaseIn(float t)
    {
        return sin((t - 1.0f) * Pi()/2.0f) + 1.0f;
    }
    
    inline float SineEaseOut(float t)
    {
        return sin(t * Pi()/2.0f);
    }
    
    inline float SineEaseInOut(float t)
    {
        return 0.5f * (1.0f - cos(t * Pi()));
    }
    
    inline float CircularEaseIn(float t)
    {
        return 1.0f - sqrt(1.0f - t * t);
    }
    
    inline float CircularEaseOut(float t)
    {
        return sqrt((2.0f - t) * t);
    }
    
    inline float CircularEaseInOut(float t)
    {
        if (t < 0.5f)
        {
            return 0.5f * (1.0f - sqrt(1.0f - 4.0f * t * t));
        }
        else
        {
            return 0.5f * sqrt(-4.0f * t * t + 8.0f * t - 3.0f) + 0.5f;
        }
    }
    
    inline float ExponentialEaseIn(float t)
    {
        return (t == 0.0f) ? t : pow(2.0f, 10.0f * (t - 1.0f));
    }
    
    inline float ExponentialEaseOut(float t)
    {
        return (t == 1.0f) ? t : 1.0f - pow(2.0f, -10.0f * t);
    }
    
    inline float ExponentialEaseInOut(float t)
    {
        if (t == 0.0f || t == 1.0f)
        {
            return t;
        }
        else if (t < 0.5f)
        {
            return 0.5f * pow(2.0f, 20.0f * t - 10.0f);
        }
        else
        {
            return 1.0f - 0.5f * pow(2.0f, -20.0f * t + 10.0f);
        }
    }
    
    inline float Smoothstep(float t)
    {
        return t * t * (3.0f - 2.0f * t);
    }
    
    inline float Smootherstep(float t)
    {
        return 6.0f * pow(t, 5.0f) - 15.0f * pow(t, 4.0f) + 10.0f * pow(t, 3.0f);
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
