//
//  Eases.hpp
//  egpfwDemoCocoa
//
//  Created by Ty Wood on 2/8/17.
//  Copyright © 2017 Dan Buckstein. All rights reserved.
//

#ifndef Eases_h
#define Eases_h

#include <stdio.h>





namespace TimingFunctions
{
    float Linear(float t)
    {
        return t;
    }
    
    float QuadraticEaseIn(float t)
    {
        return t * t;
    }
    
    float QuadraticEaseOut(float t)
    {
        return t * (2.0 - t);
    }
    
    float QuadraticEaseInOut(float t)
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
    
    float CubicEaseIn(float t)
    {
        return t * t * t;
    }
    
    float CubicEaseOut(float t) 
    {
        float f = t - 1.0;
        return 1.0 + f * f * f;
    }
    
    float CubicEaseInOut(float t)
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
    
    float QuarticEaseIn(float t)
    {
        return t * t * t * t;
    }
    
    float QuarticEaseOut(float t)
    {
        float f = t - 1.0;
        return 1.0 - f * f * f * f;
    }
    
    float QuarticEaseInOut(float t)
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
    
    float QuinticEaseIn(float t)
    {
        return t * t * t * t * t;
    }
    
    float QuinticEaseOut(float t)
    {
        float f = t - 1.0;
        return 1.0 + f * f * f * f * f;
    }
    
    float QuinticEaseInOut(float t)
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
    
    float SineEaseIn(float t)
    {
        return sin((t - 1.0) * M_PI/2) + 1.0;
    }
    
    float SineEaseOut(float t)
    {
        return sin(t * M_PI/2);
    }
    
    float SineEaseInOut(float t)
    {
        return 0.5 * (1.0 - cos(t * M_PI));
    }
    
    float CircularEaseIn(float t)
    {
        return 1.0 - sqrt(1.0 - t * t);
    }
    
    float CircularEaseOut(float t)
    {
        return sqrt((2.0 - t) * t);
    }
    
    float CircularEaseInOut(float t)
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
    
    float ExponentialEaseIn(float t)
    {
        return (t == 0.0) ? t : pow(2.0, 10.0 * (t - 1.0));
    }
    
    float ExponentialEaseOut(float t)
    {
        return (t == 1.0) ? t : 1.0 - pow(2.0, -10.0 * t);
    }
    
    float ExponentialEaseInOut(float t)
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
    
    float Smoothstep(float t)
    {
        return t * t * (3 - 2 * t);
    }
    
    float Smootherstep(float t)
    {
        return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
    }
}







template <typename T>
T lerp(T start, T end, float percent, float (*f)(float))
{
    return (start + (*f)(percent)*(end - start));
}
#endif /* Eases_h */
