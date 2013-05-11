//
//  PopupMenu.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#include "PopupMenu.h"

PopupMenu::PopupMenu(CCNode *parent)
{
    CCSprite *pineappleSprite = CCSprite::create("pineappleitem.png");
    CCSprite *pineappleSpriteSelected = CCSprite::create("pineappleitem.png");
    pineappleSpriteSelected->setColor(ccc3(100, 0, 0));
    CCMenuItemSprite *pineappleItem = CCMenuItemSprite::create(pineappleSprite, pineappleSpriteSelected, NULL, this, menu_selector(PopupMenu::createPineapple));
    
    CCSprite *ropeSprite = CCSprite::create("ropeitem.png");
    CCSprite *ropeSpriteSelected = CCSprite::create("ropeitem.png");
    CCSprite *ropeSprite3 = CCSprite::create("ropeitem.png");
    ropeSpriteSelected->setColor(ccc3(100, 0, 0));
    ropeSprite3->setColor(ccc3(100, 100, 100));
    _ropeItem = CCMenuItemSprite::create(ropeSprite, ropeSpriteSelected, ropeSprite3, this,menu_selector(PopupMenu::createRope));
    _menu = CCMenu::create(pineappleItem,_ropeItem,NULL);
    _background = CCSprite::create("menu.png");
    _background->addChild(_menu,150);
    this->addChild(_background);
    
    parent->addChild(this,1000);
    this->setMenuEnabled(false);
}

PopupMenu::~PopupMenu()
{
    
}


void PopupMenu::setPopupPositioin(CCPoint position)
{
    _tapPosition = position;
    
    // load defaultBackground and use its size to determine whether the popup still fits there

    CCSprite *defaultBackground = CCSprite::create("menu.png");
    CCSize defaultBackgroundSize = defaultBackground->getContentSize();
    float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
    float padding = defaultBackgroundSize.width * 0.1 * contentScaleFactor;
    _menu->alignItemsHorizontallyWithPadding(padding);
    
    CCPoint anchorPoint = ccp(0.5f, 0.0f);
    CCPoint menuPosition = ccp(defaultBackgroundSize.width/2, defaultBackgroundSize.height*0.7f);
    
    // Menu horizontal alignment
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    const char* horizontalAlignment;
    if (position.x < defaultBackgroundSize.width/2) {
        // left
        horizontalAlignment = "left";
        anchorPoint.x = 0.0f;
        _menu->alignItemsVerticallyWithPadding(padding);
        menuPosition.x = defaultBackgroundSize.height * 0.7f;
        menuPosition.y = defaultBackgroundSize.width * 0.5f;
    } else if (winSize.width-position.x < defaultBackgroundSize.width/2) {
        // right
        horizontalAlignment = "right";
        anchorPoint.x = 1.0f;
        _menu->alignItemsVerticallyWithPadding(padding);
        menuPosition.x = defaultBackgroundSize.height * 0.3f;
        menuPosition.y = defaultBackgroundSize.width * 0.5f;
    } else {
        // center
        horizontalAlignment = "center";
        _menu->alignItemsHorizontallyWithPadding(padding);
    }
    
    // Menu vertical alignment
    const char* verticalAlignment;
    if (position.y < defaultBackgroundSize.height) {
        // bottom
        verticalAlignment = "bottom";
        if (strcmp(horizontalAlignment, "center") == 0) {
            anchorPoint.y = 0.0f;
        } else {
            anchorPoint.y = 0.25f;
        }
    } else if (winSize.height - position.y < defaultBackgroundSize.height) {
        // top
        verticalAlignment = "top";
        if (strcmp(horizontalAlignment, "center") == 0) {
            anchorPoint.y = 1.0f;
            menuPosition.y = defaultBackgroundSize.height*0.3f;
        } else {
            anchorPoint.y = 0.75f;
        }
    } else {
        // middle
        verticalAlignment = "middle";
        if (strcmp(horizontalAlignment, "center") == 0) {
            anchorPoint.y = 0.0f;
        } else {
            anchorPoint.y = 0.5f;
        }
    }
    
    // Draw the menu
//    NSString* filename = [NSString stringWithFormat: @"menu-%@-%@.png", verticalAlignment, horizontalAlignment];
    CCString *filesname = CCString::createWithFormat("menu-%s-%s.png",verticalAlignment,horizontalAlignment);

    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(filesname->getCString());
    if (!tex) {
        tex = CCTextureCache::sharedTextureCache()->addImage("menu.png");
    }
    _background->setTexture(tex);
    _background->setTextureRect(CCRectMake(0, 0, tex->getContentSize().width, tex->getContentSize().height));
   
    
    _background->setAnchorPoint(anchorPoint);
    _background->setPosition(position);
    _background->setOpacity(_menu->getOpacity());
    
    _menu->setPosition(menuPosition);
}

void PopupMenu::setMenuEnabled(bool enable)
{
    CCObject *obj;
    
    CCARRAY_FOREACH(_menu->getChildren(), obj)
    {
        CCMenuItem *item = (CCMenuItem*)obj;
        item->setEnabled(enable);
    }
    
    _isEnabled = enable;
    

    int opacity;
    if (enable) {
        opacity = 255;
    } else {
        opacity = 0;
    }
    _background->setOpacity(opacity);
    _menu->setOpacity(opacity);
}

void PopupMenu::setRopeItemEnabled(bool enable)
{
    _ropeItem->setEnabled(_isEnabled);
}

bool PopupMenu::isEnabled()
{
    return _isEnabled;
}

void PopupMenu::createPineapple()
{
    delegate->createPineappleAt(_tapPosition);
}

void PopupMenu::createRope()
{
    delegate->createRopeAt(_tapPosition);
}