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
#include "PineappleModel.h"
#include "RopeModel.h"
#include "CoordinateHelper.h"
#include "RopeSprite.h"


#pragma mark - initialize

LevelEditor::LevelEditor(LevelFileHelper *levelHelper)
:_fileHandler(levelHelper),
_background(NULL),
_pineapplesSpriteSheet(NULL),
_ropeSpriteSheet(NULL),
_popupMenu(NULL),
_ropeSpritesArray(NULL)
{
    
}

LevelEditor::~LevelEditor()
{
    CC_SAFE_RELEASE_NULL(_ropeSpritesArray);
    CC_SAFE_DELETE(_popupMenu);
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
        
        setTouchEnabled(true);
        
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
    // Draw pineapple
    CCArray *pienapple = _fileHandler->_pineapples;
    CCObject *obj;
    CCARRAY_FOREACH(pienapple, obj)
    {
        PineappleModel *pineappleModel = (PineappleModel*)obj;
        this->createPineappleSpriteFromModel(pineappleModel);
    }

    // Draw ropes
    _ropeSpritesArray = CCArray::createWithCapacity(5);  //?
    _ropeSpritesArray->retain();
    CCARRAY_FOREACH(_fileHandler->_ropes, obj)
    {
        this->createRopeSpriteFromModel((RopeModel*)obj);
    }
    
}

void LevelEditor::createRopeSpriteFromModel(RopeModel *rm)
{
    CCPoint anchorA;
    if (rm->bodyAId == -1) {
        
        anchorA = CoordinateHelper::levelPositioinToScreenPosition(rm->achorA);
    } else {
        PineappleModel* pineappleWithID = _fileHandler->getPineappleWithID(rm->bodyAId);
        anchorA = CoordinateHelper::levelPositioinToScreenPosition(pineappleWithID->position);
    }
    
    CCPoint anchorB;
    if (rm->bodyBId == -1) {
        anchorB = CoordinateHelper::levelPositioinToScreenPosition(rm->achorB);
    } else {
        
        PineappleModel* pineappleWithID = _fileHandler->getPineappleWithID(rm->bodyBId);
        anchorB = CoordinateHelper::levelPositioinToScreenPosition(pineappleWithID->position);
    }
    

    RopeSprite *ropeSprite = new RopeSprite(_ropeSpriteSheet,rm);
    _ropeSpritesArray->addObject(ropeSprite);
    ropeSprite->release();
}

void LevelEditor::createPineappleSpriteFromModel(PineappleModel *pm)
{
    CCSprite* pineappleSprite = CCSprite::createWithSpriteFrameName("pineapple.png");
    
    pineappleSprite->setTag(pm->id);
    CCPoint position = CoordinateHelper::levelPositioinToScreenPosition(pm->position);
    pineappleSprite->setPosition(position);
    _pineapplesSpriteSheet->addChild(pineappleSprite);
}

#pragma mark - touches handler
void LevelEditor::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLOG("touch began count = %d", pTouches->count());
    //如果4个手指同时按下，则弹出菜单，取代长按的手势
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    this->togglePopupMenu(touchLocation);
    
}

void LevelEditor::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLOG("touch move count = %d", pTouches->count());

}

void LevelEditor::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLOG("touch end count = %d", pTouches->count());

}

void LevelEditor::togglePopupMenu(cocos2d::CCPoint touchLocation)
{
    if (!_popupMenu) {
        _popupMenu = new PopupMenu(this);
        _popupMenu->delegate = this;
    }
    
    if (_popupMenu->isEnabled()) {
        _popupMenu->setMenuEnabled(false);
    }else{
        _popupMenu->setPopupPositioin(touchLocation);
        _popupMenu->setMenuEnabled(true);
        if (_pineapplesSpriteSheet->getChildren()->count() < 1) {
            _popupMenu->setRopeItemEnabled(false);
        }
    }
}

#pragma mark - PopupMenuDelegate
void LevelEditor::createPineappleAt(cocos2d::CCPoint position)
{
    CCLOG("createPineappleAt");
    _popupMenu->setMenuEnabled(false);
}

void LevelEditor::createRopeAt(cocos2d::CCPoint position)
{
    CCLOG("createRopeAt");
    _popupMenu->setMenuEnabled(false);
}


