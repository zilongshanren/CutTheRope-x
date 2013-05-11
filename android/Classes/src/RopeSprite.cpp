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


float RopeSprite::distanceToPoint(cocos2d::CCPoint pt)
{
    CCPoint anchorA = CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorA);
    CCPoint anchorB = CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorB);
    float deltaX = anchorA.x-anchorB.x;
    float deltaY = anchorA.y-anchorB.y;
    float distance;
    if (deltaX == 0) {
        distance = fabs(anchorA.x - pt.x);
    } else if (deltaY == 0) {
        distance = fabs(anchorA.y - pt.y);
    } else {
        // calculate slope by dividing y-coordinate distance with x-coordinate distance
        float slope = deltaY/deltaX;
        // calculate y-intercept of rope t = y-m*x
        float yIntercept = anchorA.y - anchorA.x * slope;
        // construct line that is orthographic to rope and goes through point, we want to calculate distance for
        float slopeOrthogonal = -1/slope;
        // position.y = slopeOrthogonal * position.x + yInterceptOrthogonal => solve for yInterceptOrthogonal
        float yInterceptOrthogonal = pt.y - slopeOrthogonal * pt.x;
        // calculate interception between rope and orthogonal line
        float x = (yInterceptOrthogonal - yIntercept) / (slope - slopeOrthogonal);
        float y = slope * x + yIntercept;
        
        distance = ccpDistance(pt, CCPointMake(x, y));
    }
    return distance;
}

CCRect RopeSprite::getBoundingBox()
{
    return _ropeSprite->boundingBox();
}


void RopeSprite::cleanupSprite()
{
    _ropeSprite->removeFromParentAndCleanup(true);
}


void RopeSprite::setSelectedAnchorType(anchorType type)
{
    _selectedAnchor = type;
}
anchorType RopeSprite::getSelectedAnchorType()
{
    return _selectedAnchor;
}

CCPoint RopeSprite::getSelectedAnchor()
{
    if (_selectedAnchor == kAnchorA) {
        return CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorA);
    }
    if (_selectedAnchor == kAnchorB) {
        return CoordinateHelper::levelPositioinToScreenPosition(_ropeModel->achorB);
    }
    return CCPointMake(-1, -1);
}

void RopeSprite::moveSelectedAnchorTo(CCPoint vector)
{
    if (_selectedAnchor == kAnchorA) {
        _ropeModel->achorA = CoordinateHelper::screenPositionToLevelPosition(vector);
    }
    if (_selectedAnchor == kAnchorB) {
        _ropeModel->achorB = CoordinateHelper::screenPositionToLevelPosition(vector);
    }
    this->updateRope();
}

bool RopeSprite::isValideNewAnchorID(int newAnchorID)
{
    int unselectedAnchorID;
    if (_selectedAnchor == kAnchorA) {
        unselectedAnchorID = _ropeModel->bodyBId;
    } else {
        unselectedAnchorID = _ropeModel->bodyAId;
    }
    if (newAnchorID == unselectedAnchorID) {
        return false;
    }
    return true;
}
