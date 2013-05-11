//
//  LevelSelectionScene.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#ifndef __CutTheRopeDemo__LevelSelectionScene__
#define __CutTheRopeDemo__LevelSelectionScene__

#include "cocos2d.h"
USING_NS_CC;

class LevelSelectionScene : public CCLayer
{
public:
    LevelSelectionScene();
    ~LevelSelectionScene();
    static CCScene* scene();
    
    void selectLevel(CCObject *pSender);
    
private:
    
};

#endif /* defined(__CutTheRopeDemo__LevelSelectionScene__) */
