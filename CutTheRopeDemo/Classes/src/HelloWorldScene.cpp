//
//  HelloWorldScene.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define kCuttingSound       "cut.caf"
#define kBiteSound          "bite.caf"
#define kSplashSound        "splash.caf"
#define kBackgroundMusic    "CheeZeeJungle.caf"

#define PTM_RATIO 32

#define cc_to_b2Vec(x,y)   (b2Vec2((x)/PTM_RATIO, (y)/PTM_RATIO))


enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite()
: m_pBody(NULL)
{

}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}


// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c,  s,
        -s,    c,
        x,    y );

    return m_sTransform;
}

#pragma mark - HelloWorld

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
  

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CutTheVerlet.plist");
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("bg.png");
    bg->setAnchorPoint(CCPointZero);  //tips: 定位bg图片的技巧,顺便讲解anchor point
    this->addChild(bg,-1);
    
    
    
    // Add the croc
    croc_ = CCSprite::createWithSpriteFrameName("croc_front_mouthclosed.png");
    croc_->setAnchorPoint(ccp(1.0,0.0));  // this is also the achor point strategy
    croc_->setPosition(ccp(320,30));
    this->addChild(croc_,1);
   
    ropeSpriteSheet = CCSpriteBatchNode::create("rope_texture.png");
    this->addChild(ropeSpriteSheet);

    srand(time(NULL));
    
    // init physics
    this->initPhysics();
    
    this->initLevel();
    
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(kCuttingSound);
    SimpleAudioEngine::sharedEngine()->preloadEffect(kBiteSound);
    SimpleAudioEngine::sharedEngine()->preloadEffect(kSplashSound);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(kBackgroundMusic,true);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.4);
    
    this->schedule(SEL_SCHEDULE(&HelloWorld::changeCrocAttitude), 4.0);
    
    scheduleUpdate();
}

HelloWorld::~HelloWorld()
{
    delete world;
    world = NULL;
    
    //delete m_debugDraw;
    //TODO: clear all ropes

    
    
    CC_SAFE_DELETE(contactListener);
    CC_SAFE_DELETE(_debugDraw);
    
}

#pragma mark - physics

void HelloWorld::initPhysics()
{

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);


    _debugDraw = new GLESDebugDraw(PTM_RATIO);
    world->SetDebugDraw(_debugDraw);
    
    contactListener = new MyContactListener();
    world->SetContactListener(contactListener);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
            flags += b2Draw::e_jointBit;
            flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
            flags += b2Draw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(-s.width/PTM_RATIO,0), b2Vec2(2*s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    
    
    b2BodyDef crocBodyDef;
    crocBodyDef.position.Set((s.width - croc_->getTextureRect().size.width)/PTM_RATIO,
                             (croc_->getPosition().y)/PTM_RATIO);
    
    crocMouth_ = world->CreateBody(&crocBodyDef);
    
    // Define the croc's box shape.
    b2EdgeShape crocBox;
    
    // bottom
    crocBox.Set(b2Vec2(5.0/PTM_RATIO,15.0/PTM_RATIO), b2Vec2(45.0/PTM_RATIO,15.0/PTM_RATIO));
    crocMouthBottom_ = crocMouth_->CreateFixture(&crocBox,0);
    
    crocMouth_->SetActive(false);
}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}



void HelloWorld::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }    
    }
    
    // Update all the ropes
    
    for (int i=0; i <ropes.size(); ++i) {
        VRope *rope = ropes[i];
        rope->update(dt);
        rope->updateSprites();
    }
    
    
    // Check for collisions
    bool shouldCloseCrocMouth = false;
    std::vector<b2Body *>toDestroy;
    std::vector<MyContact>::iterator pos;
    for(pos = contactListener->_contacts.begin(); pos != contactListener->_contacts.end(); ++pos)
    {
        MyContact contact = *pos;
        
        bool hitTheFloor = false;
        b2Body *potentialCandy = NULL;
        
        // The candy can hit the floor or the croc's mouth. Let's check
        // what it's touching.
        if (contact.fixtureA == crocMouthBottom_)
        {
            potentialCandy = contact.fixtureB->GetBody();
        }
        else if (contact.fixtureB == crocMouthBottom_)
        {
            potentialCandy = contact.fixtureA->GetBody();
        }
        else if (contact.fixtureA->GetBody() == groundBody)
        {
            potentialCandy = contact.fixtureB->GetBody();
            hitTheFloor = true;
        }
        else if (contact.fixtureB->GetBody() == groundBody)
        {
            potentialCandy = contact.fixtureA->GetBody();
            hitTheFloor = true;
        }
        
        // Check if the body was indeed one of the candies
        vector<b2Body*>::iterator candyIter = std::find(candies.begin(), candies.end(), potentialCandy);
        if (potentialCandy && candyIter != candies.end())
        {
            // Set it to be destroyed
            toDestroy.push_back(potentialCandy);
            if (hitTheFloor)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(kSplashSound);
                

                CCSize s = CCDirector::sharedDirector()->getWinSize();
                _loseLabel = CCLabelTTF::create("Try Again",  "Verdana-Bold",60.0,s,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
                _loseLabel->setColor(ccc3(255, 0, 0));
                _loseLabel->setAnchorPoint(CCPointZero);
                this->addChild(_loseLabel);
                
                
                // If it hits the floor you'll remove all the physics of it and just simulate the pineapple sinking
                CCSprite *sinkingCandy = (CCSprite*)potentialCandy->GetUserData();
                
                // Sink the pineapple
                CCFiniteTimeAction *sink = CCMoveBy::create(3.0, ccp(0,-sinkingCandy->getTextureRect().size.height));
                
                
                // Remove the sprite and check if should finish the level.
                CCFiniteTimeAction *finish = CCCallFuncO::create(this,
                                                                 SEL_CallFuncO(&HelloWorld::finishAction), sinkingCandy);
                
                // Run the actions sequentially.
                sinkingCandy->runAction(CCSequence::create(sink,finish,NULL));
                
                // All the physics will be destroyed below, but you don't want the
                // sprite do be removed, so you set it to null here.
                potentialCandy->SetUserData(NULL);
            }
            else
            {
                shouldCloseCrocMouth = true;
            }
        }
    }
    
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2)
    {
        b2Body *body = *pos2;
        if (body->GetUserData() != NULL)
        {
            // Remove the sprite
            CCSprite *sprite = (CCSprite *) body->GetUserData();
          //  [self removeChild:sprite cleanup:YES];
            this->removeChild(sprite,true);
            body->SetUserData(NULL);
        }
        
        // Iterate though the joins and check if any are a rope
        b2JointEdge* joints = body->GetJointList();
        while (joints)
        {
            b2Joint *joint = joints->joint;
            
            // Look in all the ropes
            vector<VRope*>::iterator posIter = ropes.begin();
            for (int i=0; i < ropes.size(); ++i) {
                VRope* rope = ropes[i];
                if (rope->getJoint() == joint) {
                    rope->removeSprites();
                    ropes.erase( posIter + i);
                    break;
                }
            }
            
            joints = joints->next;
            world->DestroyJoint(joint);
        }
        
        // Destroy the physics body
        world->DestroyBody(body);
        
        // Removes from the candies array
        //[candies removeObject:[NSValue valueWithPointer:body]];
        candies.erase(std::find(candies.begin(), candies.end(), body));
    }
    
    if (shouldCloseCrocMouth)
    {
        // If the pineapple went into the croc's mouth, immediately closes it.
        this->changeCrocAttitude();
        
        // Check if the level should finish
        this->checkLevelFinish(false);
        SimpleAudioEngine::sharedEngine()->playEffect(kBiteSound);
    }
}

void HelloWorld::finishAction(CCObject *pSender)
{
    this->removeChild((CCNode*)pSender,true);
    this->checkLevelFinish(true);
    this->removeChild(_loseLabel,true);
}


#pragma mark - touches handler
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    static CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint pt0 = touch->getPreviousLocationInView();
    CCPoint pt1 = touch->getLocationInView();
    
    // Correct Y axis coordinates to cocos2d coordinates
    pt0.y = s.height - pt0.y;
    pt1.y = s.height - pt1.y;
    
    for (int i=0; i < ropes.size(); ++i) {
        VRope *rope = ropes[i];
        vector<VStick*> stick = rope->getSticks();
        for (int j=0; j < stick.size(); ++j) {
            VStick *pStick = stick[j];
            CCPoint pa = pStick->getPointA()->getPoint();
            CCPoint pb = pStick->getPointB()->getPoint();
            if (this->checkLineIntersection(pt0, pt1, pa, pb)) {
                //cut the rope here

                b2Body *newBodyA = this->createRopeTipBody();
                b2Body *newbodyB = this->createRopeTipBody();
                
                VRope *newRope = rope->cutRopeInStick(pStick, newBodyA, newbodyB);
                ropes.push_back(newRope);
                
                SimpleAudioEngine::sharedEngine()->playEffect(kCuttingSound);
                return;
            }
        }
    }
}



CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

#pragma mark - logic

b2Body* HelloWorld::createCandyAt(cocos2d::CCPoint pt)
{
    // Get the sprite from the sprite sheet
//    CCSprite *sprite = [CCSprite spriteWithSpriteFrameName:@"pineapple.png"];
//    [self addChild:sprite];
    CCSprite *sprite = CCSprite::createWithSpriteFrameName("pineapple.png");
    this->addChild(sprite);
    
    // Defines the body of your candy
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(pt.x/PTM_RATIO, pt.y/PTM_RATIO);
    bodyDef.userData = sprite;
    bodyDef.linearDamping = 0.3f;
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define the fixture as a polygon
    b2FixtureDef fixtureDef;
    b2PolygonShape spriteShape;
    
    b2Vec2 verts[] = {
        b2Vec2(-7.6f / PTM_RATIO, -34.4f / PTM_RATIO),
        b2Vec2(8.3f / PTM_RATIO, -34.4f / PTM_RATIO),
        b2Vec2(15.55f / PTM_RATIO, -27.15f / PTM_RATIO),
        b2Vec2(13.8f / PTM_RATIO, 23.05f / PTM_RATIO),
        b2Vec2(-3.35f / PTM_RATIO, 35.25f / PTM_RATIO),
        b2Vec2(-16.25f / PTM_RATIO, 25.55f / PTM_RATIO),
        b2Vec2(-15.55f / PTM_RATIO, -23.95f / PTM_RATIO)
    };
    
    spriteShape.Set(verts, 7);
    fixtureDef.shape = &spriteShape;
    fixtureDef.density = 30.0f;
    fixtureDef.filter.categoryBits = 0x01;
    fixtureDef.filter.maskBits = 0x01;
    body->CreateFixture(&fixtureDef);
    
    candies.push_back(body);
    
    return body;
}

void HelloWorld::createRope(b2Body *bodyA, b2Body *bodyB, b2Vec2 anchorA, b2Vec2 anchorB, float sag)
{
    b2RopeJointDef jd;
    jd.bodyA = bodyA;
    jd.bodyB = bodyB;
    jd.localAnchorA = anchorA;
    jd.localAnchorB = anchorB;
    
    // Max length of joint = current distance between bodies * sag
    float32 ropeLength = (bodyA->GetWorldPoint(anchorA) - bodyB->GetWorldPoint(anchorB)).Length() * sag;
    jd.maxLength = ropeLength;
    
    // Create joint
    b2RopeJoint *ropeJoint = (b2RopeJoint *)world->CreateJoint(&jd);
    
    VRope *newRope = new VRope(ropeJoint, ropeSpriteSheet);
    ropes.push_back(newRope);
}

void HelloWorld::initLevel()
{
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    b2Body *body1 = this->createCandyAt(ccp(s.width * 0.5, s.height * 0.7));
    
    this->createRope(groundBody, body1, cc_to_b2Vec(s.width * 0.15, s.height * 0.8),
                     body1->GetLocalCenter(), 1.1);
    
    this->createRope(body1, groundBody, body1->GetLocalCenter(), cc_to_b2Vec(s.width * 0.85,
                                                                             s.height * 0.8), 1.1);
    
    this->createRope(body1, groundBody, body1->GetLocalCenter(),
                     cc_to_b2Vec(s.width * 0.83, s.height * 0.6), 1.1);
    
    int n = 10 * 60;
    int32 velocityIterations = 8;
    int32 positionIterations = 1;
    float32 dt = 1.0 / 60.0;
    while (n--)
    {
        // Instruct the world to perform a single step of simulation.
        world->Step(dt, velocityIterations, positionIterations);
        for (int i=0; i < ropes.size(); ++i) {
            VRope* rope = ropes[i];
            rope->update(dt);
        }
    }
    
    // This last update takes care of the texture repositioning.
   // [self update:dt];
    this->update(dt);
    
    crocMouthOpened = true;
    
    
    // Add the candy
    b2Body *body2 = this->createCandyAt(ccp(s.width * 0.5, s.height));
    
    // Change the linear dumping so it swings more
    body2->SetLinearDamping(0.01);
    
    // Add a bunch of ropes
    this->createRope(groundBody, body2, cc_to_b2Vec(s.width * 0.65,
                                                    s.height + 5), body2->GetLocalCenter(), 1.0);
}

bool HelloWorld::checkLineIntersection(cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint p3, cocos2d::CCPoint p4)
{
    // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
    float denominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
    
    // In this case the lines are parallel so you assume they don't intersect
    if (denominator == 0.0f)
        return false;
    float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
    float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;
    
    if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0)
    {
        return true;
    }
    
    return false;
}

b2Body* HelloWorld::createRopeTipBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 0.5f;
    b2Body *body = world->CreateBody(&bodyDef);
    
    b2FixtureDef circleDef;
    b2CircleShape circle;
    circle.m_radius = 1.0/PTM_RATIO;
    circleDef.shape = &circle;
    circleDef.density = 10.0f;
    
    // Since these tips don't have to collide with anything
    // set the mask bits to zero
    circleDef.filter.maskBits = 0;
    body->CreateFixture(&circleDef);
    
    return body;
}

void HelloWorld::changeCrocAttitude()
{
    crocMouthOpened = !crocMouthOpened;
    
    const char* spriteName = crocMouthOpened ? "croc_front_mouthopen.png" : "croc_front_mouthclosed.png";
    
    croc_->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteName));
    
    crocMouth_->SetActive(crocMouthOpened);
}

void HelloWorld::checkLevelFinish(bool forceFinish)
{
    
    if (candies.size() == 0 || forceFinish) {
        this->finishLevel();
        
        this->scheduleOnce(schedule_selector(HelloWorld::restartGame), 2.0);
    }
    
    if (candies.size() == 0 && !forceFinish)
    {
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        _winLabel = CCLabelTTF::create("You Win!",  "Verdana-Bold",60.0,s,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        _winLabel->setColor(ccc3(255, 0, 0));
        _winLabel->setAnchorPoint(CCPointZero);
        this->addChild(_winLabel);
    }
}

void HelloWorld::restartGame()
{
    this->removeChild(_winLabel,true);
    this->initLevel();
}

void HelloWorld::finishLevel()
{
    std::set<b2Body *>toDestroy;
    
    // Destroy every rope and add the objects that should be destroyed
    
    for (int i=0; i < ropes.size(); ++i) {
        VRope* rope = ropes[i];
        rope->removeSprites();
        
        if (rope->getJoint()->GetBodyA() != groundBody) {
            toDestroy.insert(rope->getJoint()->GetBodyA());
        }
        
        if (rope->getJoint()->GetBodyB() != groundBody) {
            toDestroy.insert(rope->getJoint()->GetBodyB());
        }
        
        world->DestroyJoint(rope->getJoint());
        
        //prevent memory leaks
        delete rope;
    }
    ropes.clear();

    
    
    
    // Destroy all the objects
    std::set<b2Body *>::iterator pos;
    for(pos = toDestroy.begin(); pos != toDestroy.end(); ++pos)
    {
        b2Body *body = *pos;
        if (body->GetUserData() != NULL)
        {
            // Remove the sprite
            CCSprite *sprite = (CCSprite *) body->GetUserData();
          //  [self removeChild:sprite cleanup:YES];
            this->removeChild(sprite,true);
            body->SetUserData(NULL);
        }
        world->DestroyBody(body);
    }
    
    candies.clear();

    
    for (int i=0; i < sticks.size(); ++i) {
        CC_SAFE_DELETE(sticks[i]);
    }
    
    sticks.clear();
    

}
