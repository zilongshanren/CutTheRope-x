//
//  PopupMenu.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#ifndef __CutTheRopeDemo__PopupMenu__
#define __CutTheRopeDemo__PopupMenu__

#include "cocos2d.h"
USING_NS_CC;

class PopupMenuDelegate
{
public:
    virtual void createPineappleAt(CCPoint position) = 0;
    virtual void createRopeAt(CCPoint position) = 0;
};

class PopupMenu : public CCLayer
{
public:
    PopupMenu(CCNode* node);
    ~PopupMenu();
    
    
    PopupMenuDelegate *delegate;
    
    void setPopupPositioin(CCPoint position);
    void setMenuEnabled(bool enable);
    void setRopeItemEnabled(bool enable);
    
    bool isEnabled();
    
    void createPineapple();
    void createRope();
    
private:
    PopupMenu(); //private
    
    
    CCSprite*   _background;
    CCMenu*     _menu;
    CCMenuItem* _ropeItem;
    CCPoint     _tapPosition;
    bool        _isEnabled;
};

#endif /* defined(__CutTheRopeDemo__PopupMenu__) */
