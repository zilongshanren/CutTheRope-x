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
_ropeSpritesArray(NULL),
_newRope(NULL),
_mode(kEditMode),
_selectedObject(NULL),
_connectedRopes(NULL)
{
    
}

LevelEditor::~LevelEditor()
{
    CC_SAFE_RELEASE_NULL(_ropeSpritesArray);
    CC_SAFE_DELETE(_popupMenu);
    CC_SAFE_DELETE(_newRope);
    CC_SAFE_RELEASE_NULL(_connectedRopes);
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
    
    
    if (pTouches->count() == 2) {
        this->longPress(touchLocation);
    }else{
        switch (_mode) {
            case kEditMode:
                
                this->selectRopeAnchor(touchLocation);
                if (!_selectedObject) {
                    this->selectPineapple(touchLocation);
                }
                if (!_selectedObject && !this->ropeAtPosition(touchLocation)) {
                    this->togglePopupMenu(touchLocation);
                }
                
                break;
            case kRopeAnchorPineappleMode:
                this->selectFirstAnchorPoint(touchLocation);
                break;
            case kRopeAnchorAnyMode:
                this->selectSecondAnchorPoint(touchLocation);
                break;
        }
    }
        
}


void LevelEditor::longPress(cocos2d::CCPoint pt)
{
    this->removeRopeAtPosition(pt);
    this->removePineappleAtPosition(pt);
}

void LevelEditor::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLOG("touch move count = %d", pTouches->count());
    
    
    if (_selectedObject && _mode == kEditMode) {
        CCTouch *touch = (CCTouch*)pTouches->anyObject();
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint oldTouchLocation = touch->getPreviousLocationInView();
        oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
        
    
        RopeSprite *ropeSprite = dynamic_cast<RopeSprite*>(_selectedObject);
        if (ropeSprite != NULL) {
            ropeSprite->moveSelectedAnchorTo(touchLocation);
        }
        
        // TODO: move pineapple
        CCPoint movementVector = ccpSub(touchLocation, oldTouchLocation);
        CCSprite *selectedSprite = dynamic_cast<CCSprite*>(_selectedObject);
        if (selectedSprite != NULL) {
            
            CCPoint newPosition = ccpAdd(selectedSprite->getPosition(), movementVector);
            selectedSprite->setPosition(newPosition);
            // remember which ropes are tied to the selectedSprite
            CCObject *obj;
            CCARRAY_FOREACH(_connectedRopes, obj)
            {
                RopeSprite *rope = (RopeSprite*)obj;
                rope->moveSelectedAnchorTo(newPosition);
            }
        }
    }
}

void LevelEditor::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLOG("touch end count = %d", pTouches->count());
    if (_mode == kEditMode) {
        RopeSprite *rope = dynamic_cast<RopeSprite*>(_selectedObject);
        if (NULL != rope) {
            
            CCTouch *touch = (CCTouch*)pTouches->anyObject();
            CCPoint touchLocation = touch->getLocationInView();
            touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
            
            // snap to pineapple if rope ends up on top of it
            int newAnchorID = -1;
            CCSprite* pineappleAtTouchLocation = this->pineappleAtPosition(touchLocation);
            if (pineappleAtTouchLocation) {
                rope->moveSelectedAnchorTo(pineappleAtTouchLocation->getPosition());
                // get id of pineapple to properly set anchor in connected data model
                newAnchorID = pineappleAtTouchLocation->getTag();
            }
            
            bool isValidNewAnchor = rope->isValideNewAnchorID(newAnchorID);
            if (isValidNewAnchor) {
             
                _fileHandler->moveRopeWithId(rope->getID(), rope->getSelectedAnchorType(), newAnchorID,
                                             CoordinateHelper::screenPositionToLevelPosition(touchLocation));
            } else {
                _fileHandler->moveRopeWithId(rope->getID(), rope->getSelectedAnchorType(),
                                             rope->getSelectedAnchorType(),
                                             CoordinateHelper::screenPositionToLevelPosition(_originalRopeAnchor));
                
                rope->moveSelectedAnchorTo(_originalRopeAnchor);
            }
            
            // set selected anchor to none
            //[rope setSelectedAnchorType:kAnchorNone];
            rope->setSelectedAnchorType(kAnchorNone);
        }
        
        // TODO: end moving pineapple
        CCSprite *pineappleSprite = dynamic_cast<CCSprite*>(_selectedObject);
        if (pineappleSprite != NULL) {
            int pineappleID = pineappleSprite->getTag();
            CCPoint newPineapplePosition = CoordinateHelper::screenPositionToLevelPosition(pineappleSprite->getPosition());
            
            _fileHandler->movePineappleWithID(pineappleID, newPineapplePosition);
        }
        if (_connectedRopes != NULL) {
            _connectedRopes->removeAllObjects();
        }
        
        _selectedObject = NULL;
    }

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

    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    PineappleModel *pm = _fileHandler->addPineappleAt(ccp(position.x / winSize.width,
                                                          position.y / winSize.height));
    
    this->createPineappleSpriteFromModel(pm);
    _popupMenu->setMenuEnabled(false);

}

void LevelEditor::createRopeAt(cocos2d::CCPoint position)
{
    CCLOG("createRopeAt");
    this->setMode(kRopeAnchorPineappleMode);
    _newRope = new RopeModel;
    _popupMenu->setMenuEnabled(false);
}

#pragma mark - others
void LevelEditor::setMode(editorMode newMode)
{
    _mode = newMode;
    CCObject *obj;

    
    switch (_mode) {
        case kRopeAnchorPineappleMode:
            _background->setColor(kDefaultDisabledColor);
            
            CCARRAY_FOREACH(_pineapplesSpriteSheet->getChildren(), obj)
            {
                CCSprite *pineapple = (CCSprite*)obj;
                pineapple->setColor(kDefaultSelectableColor);
            }
            
            
            break;
        case kRopeAnchorAnyMode:
            _background->setColor(kDefaultSelectableColor);
            
            CCARRAY_FOREACH(_pineapplesSpriteSheet->getChildren(), obj)
            {
                CCSprite *pineapple = (CCSprite*)obj;
                if (pineapple->getTag() == _newRope->bodyAId) {
                    pineapple->setColor(kDefaultDisabledColor);
                }else{
                    pineapple->setColor(kDefaultSelectableColor);
                }
            }
            break;
        case kEditMode:
        default:
            _background->setColor(kDefaultBackgroundColor) ;
            CCARRAY_FOREACH(_pineapplesSpriteSheet->getChildren(), obj)
            {
                CCSprite *pineapple = (CCSprite*)obj;
                pineapple->setColor(ccc3(255, 255, 255));
            }
            
            break;
    }
}

CCSprite* LevelEditor::pineappleAtPosition(cocos2d::CCPoint pt)
{
    CCObject *obj;
    CCARRAY_FOREACH(_pineapplesSpriteSheet->getChildren(), obj)
    {
        CCSprite *pineapple = (CCSprite*)obj;
        if (pineapple->boundingBox().containsPoint(pt)) {
            return pineapple;
        }
    }
    return NULL;
}

void LevelEditor::selectFirstAnchorPoint(cocos2d::CCPoint touchLocation)
{
    // if user tapped on pineapple set it as anchor of the new rope
    CCSprite *tappedPineapple = this->pineappleAtPosition(touchLocation);
    
    if (tappedPineapple != NULL) {
        _newRope->achorA = CoordinateHelper::screenPositionToLevelPosition(tappedPineapple->getPosition());
        _newRope->bodyAId = tappedPineapple->getTag();
        this->setMode(kRopeAnchorAnyMode);
    }
}


void LevelEditor::selectSecondAnchorPoint(cocos2d::CCPoint touchLocation)
{
    // set second end of rope, can be either background or other pinapple, but not same pinapple as first one

    CCSprite *tappedPineapple = this->pineappleAtPosition(touchLocation);
    if (tappedPineapple && tappedPineapple->getTag() != _newRope->bodyAId) {
        _newRope->achorB = CoordinateHelper::screenPositionToLevelPosition(tappedPineapple->getPosition());
        _newRope->bodyBId = tappedPineapple->getTag();
    }
    
    if (!tappedPineapple) {
        _newRope->achorB = CoordinateHelper::screenPositionToLevelPosition(touchLocation);
        _newRope->bodyBId = -1;
    }
    
    this->createRopeSpriteFromModel(_newRope);
    _fileHandler->addRopeFromModel(_newRope);
    
    this->setMode(kEditMode);
}

RopeSprite* LevelEditor::ropeAtPosition(cocos2d::CCPoint position)
{
    float minDistanceToRope = 5;
    RopeSprite* closestRope = NULL ;
    CCObject *obj;
    CCARRAY_FOREACH(_ropeSpritesArray, obj)
    {
        RopeSprite *ropeSprite = (RopeSprite*)obj;
        if (ropeSprite->getBoundingBox().containsPoint(position)
            && ropeSprite->distanceToPoint(position)) {
            closestRope = ropeSprite;
            minDistanceToRope = ropeSprite->distanceToPoint(position);
        }
    }
    return closestRope;
}

void LevelEditor::removeRopeAtPosition(cocos2d::CCPoint position)
{
    RopeSprite *selectedRope = this->ropeAtPosition(position);
    if (selectedRope) {
        selectedRope->cleanupSprite();
        _fileHandler->removeRopeWithID(selectedRope->getID());
        _ropeSpritesArray->removeObject(selectedRope);
        return;
    }
}

void LevelEditor::removePineappleAtPosition(cocos2d::CCPoint positioin)
{
    CCSprite* selectedPineapple = this->pineappleAtPosition(positioin);
    if (selectedPineapple != NULL) {
        _fileHandler->removePineappleWithID(selectedPineapple->getTag());
        
        CCArray *connectedRopesToBeRemoved = this->getAllRopesConnectedToPineappleWithID(selectedPineapple->getTag());
        
        CCObject *obj;
        CCARRAY_FOREACH(connectedRopesToBeRemoved, obj)
        {
            RopeSprite *rope = (RopeSprite*)obj;
            _fileHandler->removeRopeWithID(rope->getID());
            rope->cleanupSprite();
            _ropeSpritesArray->removeObject(rope);
        }
        
        selectedPineapple->removeFromParentAndCleanup(true);
        
    }
}

CCArray* LevelEditor::getAllRopesConnectedToPineappleWithID(int pineappleID)
{
    return this->getAllRopesConnectedToPineappleWithID(pineappleID,false);
}

void LevelEditor::selectRopeAnchor(cocos2d::CCPoint touchLocation)
{
    CCObject *obj;
    CCARRAY_FOREACH(_ropeSpritesArray, obj)
    {
        RopeSprite *rope = (RopeSprite*)obj;
        float distanceToAnchorA = ccpDistance(rope->getAnchorA(), touchLocation);
        if (distanceToAnchorA < 10) {
            _selectedObject = rope;
            rope->setSelectedAnchorType(kAnchorA);
            _originalRopeAnchor = rope->getAnchorA();
        }
        
        float distanceToAnchorB = ccpDistance(rope->getAnchorB(), touchLocation);
        if (distanceToAnchorB < 10) {
            _selectedObject = rope;
            rope->setSelectedAnchorType(kAnchorB);
            _originalRopeAnchor = rope->getAnchorB();
        }
        
        if (_selectedObject) {
            break;
        }
    }
}

void LevelEditor::selectPineapple(cocos2d::CCPoint touchLocation)
{
    CCSprite* sprite = this->pineappleAtPosition(touchLocation);
    if (sprite) {
        _selectedObject = sprite;
        // remember which ropes are tied to the selectedSprite
        _connectedRopes = this->getAllRopesConnectedToPineappleWithID(sprite->getTag(), true);
        _connectedRopes->retain();
    }
}

CCArray* LevelEditor::getAllRopesConnectedToPineappleWithID(int pineappleID, bool setSelectedAnchor)
{
    CCArray *tempRopes = CCArray::create();
    CCObject *obj;
    CCARRAY_FOREACH(_ropeSpritesArray, obj)
    {
        RopeSprite* rope = (RopeSprite*)obj;
        int bodyAID = rope->getBodyAID();
        int bodyBID = rope->getBodyBID();
        
        if (pineappleID == bodyAID || pineappleID == bodyBID) {
            tempRopes->addObject(rope);
        }
        
        if (setSelectedAnchor) {
            if (pineappleID == bodyAID) {
                rope->setSelectedAnchorType(kAnchorA);
            } else if (pineappleID == bodyBID) {
                rope->setSelectedAnchorType(kAnchorB);
            }
        }
    }
    
    return tempRopes;
}

