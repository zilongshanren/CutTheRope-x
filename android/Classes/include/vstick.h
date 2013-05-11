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
//  vstick.cpp
//
//  Created by patrick on 14/10/2010.
//  Modified by Flightless www.flightless.co.nz 20/4/2012
//  Ported to cocos2d-x by Creeng www.creeng.com 12/7/2012
//

#ifndef VSTICK_H
#define VSTICK_H

#include "cocos2d.h"

#include "vpoint.h"

class VStick
{
public:
    VStick();
    VStick(VPoint* argA, VPoint* argB);
    virtual ~VStick();

    void contract();
    VPoint* getPointA();
    VPoint* getPointB();
    void setPointB(VPoint *pt);
    void setPointA(VPoint *pt);

private:

	VPoint *pointA;
	VPoint *pointB;
	float hypotenuse;

};

#endif // VSTICK_H