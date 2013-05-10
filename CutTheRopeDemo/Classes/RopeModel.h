//
//  RopeModel.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#ifndef __CutTheRopeDemo__RopeModel__
#define __CutTheRopeDemo__RopeModel__

#include "AbstractModel.h"

class RopeModel : public AbstractModel
{
public:
    RopeModel();
    
    // The position of each of the rope ends.
    // If an end is connected to a pineapple, then this property is ignored
    // and the position of the pineapple is used instead.
    CCPoint achorA;
    CCPoint achorB;
    
    // ID of the body the rope is connected to. -1 refers to the background.
    // all other IDs refer to pineapples distributed in the level
    int bodyAId;
    int bodyBId;
    
    // The sagginess of the line
    float sagity;
};

#endif /* defined(__CutTheRopeDemo__RopeModel__) */
