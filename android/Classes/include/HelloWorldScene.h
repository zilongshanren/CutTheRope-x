//
//  HelloWorldScene.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "vrope.h"
#include "MyContactListener.h"

USING_NS_CC;

class GLESDebugDraw;
class LevelFileHelper;

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld(LevelFileHelper* levelHelper);
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    static CCScene *HelloWorldSceneWithLevelHandler(LevelFileHelper* levelHelper);
    
    
    void initPhysics();
    // adds a new sprite at a given coordinate

    virtual void draw();
    virtual void ccTouchesMoved(CCSet *touches, CCEvent* event);
    void update(float dt);
    b2Body* createCandyAt(CCPoint pt);
    void createRope(b2Body* bodyA, b2Body* bodyB, b2Vec2 anchorA, b2Vec2 anchorB,float sag);
    void initLevel();
    bool checkLineIntersection(CCPoint p1, CCPoint p2, CCPoint p3, CCPoint p4);
    b2Body* createRopeTipBody();
    void changeCrocAttitude();
    void checkLevelFinish(bool forceFinish);
    void finishLevel();
    
    //action callbacks
    void finishAction(CCObject *pSender);
    void restartGame();
    void switchToEditor();
    void switchToMenu();
    
    
    //init
    void createMenu();
private:
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    CCSprite *croc_;            // weak ref
    
    vector<VRope*> _ropes;
    vector<b2Body*> _candies;
    vector<VStick*> _sticks;
    
    b2Body* groundBody;    // weak ref
    CCSpriteBatchNode *_ropeSpriteSheet; // weak ref
    GLESDebugDraw   *_debugDraw;
    
    b2Body *crocMouth_;          // weak ref
    b2Fixture *crocMouthBottom_;    // weak ref
    
    bool crocMouthOpened;
    MyContactListener *contactListener;
    CCLabelTTF *_loseLabel;
    CCLabelTTF *_winLabel;
    
    LevelFileHelper     *_levelEditor;
};

#endif // __HELLO_WORLD_H__
