//
//  Constants.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#ifndef __CutTheRopeDemo__Constants__
#define __CutTheRopeDemo__Constants__

#include "cocos2d.h"
USING_NS_CC;

extern float const kDefaultDamping;
extern float const kDefaultSagity;

extern ccColor3B kDefaultBackgroundColor;
extern ccColor3B kDefaultSelectableColor;
extern ccColor3B kDefaultDisabledColor;

enum {
    kAnchorNone,
    kAnchorA,
    kAnchorB
} typedef anchorType;

#endif /* defined(__CutTheRopeDemo__Constants__) */
