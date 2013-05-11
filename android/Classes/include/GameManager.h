//
//  GameManager.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#ifndef __CutTheRopeDemo__GameManager__
#define __CutTheRopeDemo__GameManager__

#include "cocos2d.h"

USING_NS_CC;

class GameManager
{
public:
    static GameManager* getInstance();
    
    int levelNumber;
    
private:
    static GameManager *_pGameManager;
    
    GameManager(){}
    GameManager(const GameManager& other);
void operator=(const GameManager& other);

};

#endif /* defined(__CutTheRopeDemo__GameManager__) */
