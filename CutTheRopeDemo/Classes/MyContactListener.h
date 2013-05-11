//
//  MyContactListener.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#ifndef __CutTheRopeDemo__MyContactListener__
#define __CutTheRopeDemo__MyContactListener__

#include "Box2D.h"
#include <vector>
#include <algorithm>

struct MyContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};
    
    class MyContactListener : public b2ContactListener {
        
    public:
        std::vector<MyContact>_contacts;
        
        MyContactListener();
        ~MyContactListener();
        
        virtual void BeginContact(b2Contact* contact);
        virtual void EndContact(b2Contact* contact);
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        
    };

#endif /* defined(__CutTheRopeDemo__MyContactListener__) */
