//
//  LevelSelectionScene.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#include "LevelSelectionScene.h"
#include "LevelFileHandler.h"
#include "HelloWorldScene.h"
#include "GameManager.h"

LevelSelectionScene::LevelSelectionScene()
{
    CCArray *items = CCArray::create();
    
    int levelNumber = 0;
    bool fileExists;
    do {
        CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
        const char *levelName = CCString::createWithFormat("level%d.xml",levelNumber)->getCString();
        fileExists = fileUtils->isFileExist(fileUtils->getWritablePath() + levelName);
        CCMenuItem* item;
        if (fileExists) {
            CCString* itemLabel = CCString::createWithFormat("Level %d",levelNumber);
            item = CCMenuItemFont::create(itemLabel->getCString(), this, menu_selector(LevelSelectionScene::selectLevel));
        } else {
            item = CCMenuItemFont::create("New Level", this, menu_selector(LevelSelectionScene::selectLevel));
        }
        item->setTag(levelNumber);
        items->addObject(item);
        
        levelNumber++;
    } while (fileExists);
    
    CCMenu* menu = CCMenu::createWithArray(items);
    menu->alignItemsVertically();
    menu->setPosition(ccp(160,240));
    
    this->addChild(menu);
    

}

void LevelSelectionScene::selectLevel(cocos2d::CCObject *pSender)
{
    CCMenuItem* item = (CCMenuItem*) pSender;
    int levelNumber = item->getTag();
    CCLOG("n=%d",levelNumber);
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    const char *levelName = CCString::createWithFormat("level%d.xml",levelNumber)->getCString();
    bool fileExists = fileUtils->isFileExist(fileUtils->getWritablePath() + levelName);
    
    LevelFileHelper *fileHandler;
    if (fileExists) {
        fileHandler = new LevelFileHelper((fileUtils->getWritablePath() + levelName).c_str());
    }else{
        fileHandler = new LevelFileHelper(fileUtils->fullPathForFilename("level0.xml").c_str());
    }
    
    GameManager::getInstance()->levelNumber = levelNumber;
    
    CCDirector::sharedDirector()->replaceScene(HelloWorld::HelloWorldSceneWithLevelHandler(fileHandler));

}

LevelSelectionScene::~LevelSelectionScene()
{
    
}

CCScene* LevelSelectionScene::scene()
{
    CCScene *sc = CCScene::create();
    
    LevelSelectionScene *layer = new LevelSelectionScene();
    
    sc->addChild(layer);
    
    return sc;
}