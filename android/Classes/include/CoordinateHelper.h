//
//  CoordinateHelper.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#ifndef __CutTheRopeDemo__CoordinateHelper__
#define __CutTheRopeDemo__CoordinateHelper__

#include "cocos2d.h"

USING_NS_CC;

class CoordinateHelper {
public:
    static CCPoint screenPositionToLevelPosition(CCPoint pt);
    static CCPoint levelPositioinToScreenPosition(CCPoint pt);
};

#endif /* defined(__CutTheRopeDemo__CoordinateHelper__) */
