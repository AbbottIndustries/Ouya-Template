#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
#include "VisibleRect.h"

#include "ControlNotifications.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_OUYA)
#include "platform/ouya/CCOuyaController.h"
#include <jni.h>
#endif

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void update(float fDelta);
    void menuCloseCallback(CCObject* pSender);

    void RegisterForEvents();
    void UnregisterEvents();
    
    CREATE_FUNC(HelloWorld);
    
    // My own demo stuff.    
    virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
	virtual void registerWithTouchDispatcher();
    
	int animationID;
	cocos2d::extension::CCArmature *armature;
    void dataLoaded();
    
    CCParallaxNode *_backgroundNode;
    CCSprite *_spacedust1;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    
    bool holdingMoveKey;
    bool holdingMoveKeyModifier;

    float directionMultiplier;
    
    void initBackground();
    void scrollBackground( float );
    void moveCharacterHorizontally( cocos2d::CCBool* );
    void moveCharacterVertically();
    void stopCharacterMove();
    void RunHorizontally( cocos2d::CCBool* );
    void fireLaser();
    void preloadSprites();
    void ignoringInput( cocos2d::CCArray* );
};

#endif // __HELLOWORLD_SCENE_H__
