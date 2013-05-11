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
//  vpoint.cpp
//
//  Created by patrick on 14/10/2010.
//  Modified by Flightless www.flightless.co.nz 20/4/2012
//  Ported to cocos2d-x by Creeng www.creeng.com 12/7/2012
//


#include "vpoint.h"

static float vPointGravityX = 0.0f;
static float vPointGravityY = 9.8f;

using namespace cocos2d;

VPoint::VPoint()
    : x(0.f), y(0.f), oldx(0.f), oldy(0.f)
{
}


VPoint::~VPoint()
{
}

void VPoint::setPos(float ax, float ay)
{
	x = oldx = ax;
	y = oldy = ay;
}

void VPoint::update()
{
	float tempx = x;
	float tempy = y;
	x += x - oldx;
	y += y - oldy;
	oldx = tempx;
	oldy = tempy;
}

void VPoint::applyGravity(float dt)
{
	x -= vPointGravityX*dt;
	y -= vPointGravityY*dt;
}

void VPoint::applyGravity(float dt, float gx, float gy)
{
	x -= gx*dt;
    y -= 10.0f*dt; //gravity magic number
}

void VPoint::applyGravityxdt(float gxdt, float gydt)
{
    x -= gxdt;
    y -= gydt;
}

void VPoint::setGravityX(float gx, float gy)
{
	vPointGravityX = gx;
	vPointGravityY = gy;        
}

CCPoint VPoint::getGravity()
{
    return CCPointMake(vPointGravityX, vPointGravityY);
}

