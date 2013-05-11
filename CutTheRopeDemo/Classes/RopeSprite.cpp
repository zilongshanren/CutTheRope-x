//
//  RopeSprite.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/11/13.
//
//

#include "RopeSprite.h"
#include "CoordinateHelper.h"

#pragma mark - initialize
RopeSprite::RopeSprite(CCNode *parent, RopeModel *ropeModel)
{
    _ropeModel = ropeModel;
    
    _ropeSprite = CCSprite::create("rope_texture.png");
    ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_CLAMP_TO_EDGE};
    _ropeSprite->getTexture()->setTexParameters(&params);

    this->updateRope();
    parent->addChild(_ropeSprite);
}

RopeSprite::~RopeSprite()
{
    
}



#pragma mark - logic
int RopeSprite::getID()
{
    return _ropeModel->id;
}
CCPoint RopeSprite::getAnchorA()
{
    return CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorA);
}
CCPoint RopeSprite::getAnchorB()
{
    return CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorB);
}
int RopeSprite::getBodyAID()
{
    return _ropeModel->bodyAId;
}
int RopeSprite::getBodyBID()
{
    return _ropeModel->bodyBId;
}

void RopeSprite::setAnchorA(CCPoint pt)
{
    _ropeModel->achorA = CoordinateHelper::screenPositionToLevelPosition(pt);
    this->updateRope();
}
void RopeSprite::setAnchorB(CCPoint pt)
{
    _ropeModel->achorB = CoordinateHelper::screenPositionToLevelPosition(pt);
    this->updateRope();
}

void RopeSprite::updateRope()
{
    CCPoint anchorA = CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorA);
    CCPoint anchorB = CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorB);
    

    float distance = ccpDistance(anchorA, anchorB);
    CCPoint stickVector = ccpSub(ccp(anchorA.x,anchorA.y),ccp(anchorB.x,anchorB.y));
    float angle = ccpToAngle(stickVector);
    _ropeSprite->setTextureRect(CCRectMake(0, 0, distance, _ropeSprite->getTexture()->getPixelsHigh())) ;
    _ropeSprite->setPosition(ccpMidpoint(anchorA, anchorB)) ;
    _ropeSprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));
}

