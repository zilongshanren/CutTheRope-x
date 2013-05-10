//
//  CoordinateHelper.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#include "CoordinateHelper.h"

CCPoint CoordinateHelper::screenPositionToLevelPosition(cocos2d::CCPoint pt)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    return ccp(pt.x / winSize.width, pt.y / winSize.height);
}

CCPoint CoordinateHelper::levelPositioinToScreenPosition(cocos2d::CCPoint pt)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    return ccp(pt.x * winSize.width, pt.y * winSize.height);
}