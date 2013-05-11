//
//  CutTheRopeDemoAppDelegate.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "LevelFileHandler.h"
#include "LevelSelectionScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    
    //specify resource loading strategy
    CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    CCLOG("screenSize x=%f, y=%f",screenSize.width, screenSize.height);
    
    CCSize designSize = CCSizeMake(320, 480);
    CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
    std::vector<std::string> searchPaths;
    CCSize resourceSize;
    
    if (screenSize.width > 320)
    {
        searchPaths.push_back("hd");
        searchPaths.push_back("sd");
        resourceSize = CCSizeMake(640, 960);
        // for iphone 5
        if (screenSize.height > 960) {
            designSize.height = 1136 / 2.0;
        }
    }
    else
    {
        searchPaths.push_back("sd");
        resourceSize = CCSizeMake(320, 480);
    }
    
    pFileUtils->setSearchPaths(searchPaths);
    pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
    
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit);

    

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = LevelSelectionScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
