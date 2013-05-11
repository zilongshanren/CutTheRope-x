//
//  GameManager.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#include "GameManager.h"

GameManager* GameManager::_pGameManager = NULL;

GameManager* GameManager::getInstance()
{
    if (_pGameManager == NULL) {
        _pGameManager = new GameManager;
    }
    return _pGameManager;
}