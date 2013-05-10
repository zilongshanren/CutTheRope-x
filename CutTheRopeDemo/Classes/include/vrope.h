/*
 
 MIT License.

 Copyright (c) 2012 Creeng Ltd.
 Copyright (c) 2012 Flightless Ltd.  
 Copyright (c) 2010 Clever Hamster Games.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

//
//  vrope.cpp
//
//  Created by patrick on 14/10/2010.
//  Modified by Flightless www.flightless.co.nz 20/4/2012
//  Ported to cocos2d-x by Creeng www.creeng.com 12/7/2012
//

#ifndef VROPE_H
#define VROPE_H

#include "vpoint.h"
#include "vstick.h"
#include "cocos2d.h"
#include "Box2D.h"

#include <vector>

//PTM_RATIO defined here is for testing purposes, it should obviously be the same as your box2d world or, better yet, import a common header where PTM_RATIO is defined
#define PTM_RATIO 32

class VRope
{
public:
    //VRope(b2Body* body1, b2Body* body2, cocos2d::CCSpriteBatchNode* ropeBatchNode);
    VRope(b2RopeJoint* joint, cocos2d::CCSpriteBatchNode* ropeBatchNode);
    VRope(const cocos2d::CCPoint& pointA,
          const cocos2d::CCPoint& pointB, cocos2d::CCSpriteBatchNode* ropeBatchNode);
     VRope(b2RopeJoint* joint,cocos2d::CCSpriteBatchNode* ropeBatchNode,std::vector<VPoint*> points,std::vector<VStick*> sticks,std::vector<cocos2d::CCSprite*> sprites);
    
    virtual ~VRope();

    void update(float dt);
    void updateWithPreIntegratedGravity(float dt); // Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
    void updateWithPreIntegratedGravity(float dt, float gravityX, float gravityY); // Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
    void updateWithPreIntegratedOriginGravity(float dt); // Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
    void reset();
    
    void createRope(const cocos2d::CCPoint& pointA, const cocos2d::CCPoint& pointB,float distance);
    void resetWithPoints(const cocos2d::CCPoint &pointA, const cocos2d::CCPoint& pointB);
    void updateWithPoints(const cocos2d::CCPoint& pointA, const cocos2d::CCPoint& pointB, float dt);
    void updateWithPoints(const cocos2d::CCPoint& pointA, const cocos2d::CCPoint& pointB, float gxdt, float gydt); // Flightless, update with support for pre-integrating the gravity each step (optimised for changing gravity)
    void debugDraw();
    void updateSprites();
    void removeSprites();
    VRope* cutRopeInStick(VStick* stick, b2Body* bodyA, b2Body* bodyB);
    
    std::vector<VStick*>& getSticks()
    {
        return vSticks;
    }
    
    b2RopeJoint* getJoint()
    {
        return joint_;
    }

private:
	int numPoints;
	
    std::vector<VPoint*> vPoints;
    std::vector<VStick*> vSticks;
	std::vector<cocos2d::CCSprite*> ropeSprites;
	
    cocos2d::CCSpriteBatchNode* spriteSheet;
	float antiSagHack;
	b2RopeJoint *joint_;
};

#endif  // VROPE_H