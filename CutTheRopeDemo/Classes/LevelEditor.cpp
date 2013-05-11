//
//  LevelEditor.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#include "LevelEditor.h"
#include "LevelFileHandler.h"
#include "Constants.h"
#include "HelloWorldScene.h"


#pragma mark - initialize

LevelEditor::LevelEditor(LevelFileHelper *levelHelper)
:_fileHandler(levelHelper),
_background(NULL),
_pineapplesSpriteSheet(NULL),
_ropeSpriteSheet(NULL)
{
    
}

LevelEditor::~LevelEditor()
{
    
}

CCScene* LevelEditor::createWithLevel(LevelFileHelper *levelHelper)
{
    CCScene *sc = CCScene::create();
    LevelEditor *le = new LevelEditor(levelHelper);
    if (le) {
        le->init();
        sc->addChild(le);
        
        return sc;
    }
    return NULL;
}

bool LevelEditor::init()
{
    bool bRet = false;
    
    do {
        
        //do init here
        CC_BREAK_IF(!CCLayer::init());
        
        this->createMenu();
        
        _background = CCSprite::createWithSpriteFrameName("bg.png");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        _background->setPosition(ccp(winSize.width/2, winSize.height/2));
        _background->setColor(kDefaultBackgroundColor);
        this->addChild(_background);
        
        // Load the sprite sheet into the sprite cache
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CutTheVerlet.plist");
        _pineapplesSpriteSheet = CCSpriteBatchNode::create("CutTheVerlet.pvr.ccz");
        this->addChild(_pineapplesSpriteSheet);
        _ropeSpriteSheet = CCSpriteBatchNode::create("rope_texture.png");
        this->addChild(_ropeSpriteSheet);
        
        this->drawLoadedLevel();
        
        bRet = true;
    } while (0);
    
    return bRet;
}


#pragma mark - logic
void LevelEditor::createMenu()
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *saveLabel = CCLabelTTF::create("Save", "Marker Felt", 24);
    CCMenuItem  *saveItem = CCMenuItemLabel::create(saveLabel, this, menu_selector(LevelEditor::saveLevel));
    
    CCLabelTTF *resetLabel = CCLabelTTF::create("Reset", "Marker Felt", 24);
    CCMenuItemLabel *resetItem = CCMenuItemLabel::create(resetLabel, this, menu_selector(LevelEditor::resetLevel));
    
    CCLabelTTF *playLabel = CCLabelTTF::create("Play Level", "Marker Felt", 24);
    CCMenuItemLabel *playLevelItem = CCMenuItemLabel::create(playLabel, this, menu_selector(LevelEditor::playLevel));
    CCMenu *menu = CCMenu::create(saveItem,resetItem,playLevelItem,NULL);
    menu->setPosition(winSize.width/2, saveItem->getContentSize().height/2);
    this->addChild(menu,100);
    menu->alignItemsHorizontallyWithPadding(50);
    
    
}

void LevelEditor::resetLevel()
{
    CCLOG("reset level");
}


void LevelEditor::playLevel()
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::HelloWorldSceneWithLevelHandler(_fileHandler));
}

void LevelEditor::saveLevel()
{
    CCLOG("saveLevel");
}

void LevelEditor::drawLoadedLevel()
{
    
}
