//
//  Utils.h
//  egpfwDemoCocoa
//
//  Created by Ty Wood on 2/8/17.
//  Copyright © 2017 Dan Buckstein. All rights reserved.
//

#ifndef Utils_h
#define Utils_h



float randomDelta(float a)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float r = random * a;
    
    return a + r;
}

float randomDeltaPosNeg(float a)
{
    return ((float(rand()) / float(RAND_MAX)) * (2 * a)) - a;
}



#endif /* Utils_h */
