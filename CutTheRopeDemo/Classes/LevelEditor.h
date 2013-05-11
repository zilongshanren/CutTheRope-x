//
//  LevelEditor.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#ifndef __CutTheRopeDemo__LevelEditor__
#define __CutTheRopeDemo__LevelEditor__

#include "cocos2d.h"
#include "PopupMenu.h"

USING_NS_CC;

enum {
    kEditMode,
    kRopeAnchorPineappleMode,
    kRopeAnchorAnyMode
} typedef editorMode;

class LevelFileHelper;
class PineappleModel;
class RopeModel;

class LevelEditor : public CCLayer, public PopupMenuDelegate
{
public:
    LevelEditor(LevelFileHelper *levelHelper);
    ~LevelEditor();
    static CCScene* createWithLevel(LevelFileHelper *levelHelper);
    
    virtual bool init();
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
//    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
    virtual void createPineappleAt(CCPoint position);
    virtual void createRopeAt(CCPoint position);
    
    void createMenu();
    void resetLevel();
    void saveLevel();
    void playLevel();
    void drawLoadedLevel();
    void createPineappleSpriteFromModel(PineappleModel *pm);
    void createRopeSpriteFromModel(RopeModel *rm);
    
    
    void togglePopupMenu(CCPoint touchLocation);
    
    void setMode(editorMode mode);
    CCSprite *pineappleAtPosition(CCPoint pt);
    void selectFirstAnchorPoint(CCPoint touchLocation);
    void selectSecondAnchorPoint(CCPoint touchLocation);
    
private:
    LevelEditor();
    
    LevelFileHelper         *_fileHandler;
    CCSprite                *_background;
    CCSpriteBatchNode       *_pineapplesSpriteSheet;
    CCSpriteBatchNode       *_ropeSpriteSheet;
    
    CCArray                 *_ropeSpritesArray;
    PopupMenu               *_popupMenu;
    
    editorMode              _mode;
    RopeModel*              _newRope;
};

#endif /* defined(__CutTheRopeDemo__LevelEditor__) */
