#include "HelloWorldScene.h"
#include "support/CCPointExtension.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_OUYA)
#include "Platform/OUYA/Controls/GameControls.h"
#include "platform/ouya/jni/Java_com_levire_ouyabind_Controller.h"
#include "platform/ouya/CCOuyaController.h"
#include <jni.h>
#endif

USING_NS_CC;

#define CONTROL_LAYER_TAG 100

CCScene* HelloWorld::scene()
{
    CCScene* scene = CCScene::create();
    scene->addChild( HelloWorld::create() );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_OUYA)
    scene->addChild( aiGameControls::create(), 1, CONTROL_LAYER_TAG );
#endif

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    CCLayer::init();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    this->initBackground();
    this->scheduleUpdate();

    this->RegisterForEvents();
    
    return true;
}

void HelloWorld::initBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 1) Create the CCParallaxNode
    _backgroundNode = CCParallaxNode::create(); //1
    this->addChild(_backgroundNode,-1);
    
    // 2) Create the sprites will be added to the CCParallaxNode
    _spacedust1 = CCSprite::create("bg_front_spacedust.png");
    _spacedust2 = CCSprite::create("bg_front_spacedust.png");
    _planetsunrise = CCSprite::create("bg_planetsunrise.png");
    _galaxy = CCSprite::create("bg_galaxy.png");
    _spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");
    
    // 3) Determine relative movement speeds for space dust and background
    CCPoint dustSpeed = ccp(0.1, 0.1);
    CCPoint bgSpeed = ccp(0.05, 0.05);
    
    // 4) Add children to CCParallaxNode
    _backgroundNode->addChild(_spacedust1, 0, dustSpeed, ccp(0,winSize.height/2) ); // 2
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp( _spacedust1->getContentSize().width,winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0, winSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1 , bgSpeed, ccp(600, winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));

    directionMultiplier = 1.0f;
}

void HelloWorld::RegisterForEvents()
{
    CCNotificationCenter* notifier = cocos2d::CCNotificationCenter::sharedNotificationCenter();
    notifier->addObserver( this, callfuncO_selector( HelloWorld::moveCharacterHorizontally ), MOVE_HORIZONTALLY, NULL );
    notifier->addObserver( this, callfuncO_selector( HelloWorld::stopCharacterMove ), STOP_MOVE, NULL );
    notifier->addObserver( this, callfuncO_selector( HelloWorld::RunHorizontally ), RUN_HORIZONTALLY, NULL );
    notifier->addObserver( this, callfuncO_selector( HelloWorld::fireLaser ), FIRE_LASER, NULL );
}

void HelloWorld::onEnter()
{
    CCLayer::onEnter();

    setTouchEnabled(true);

    cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("robot.png", "robot.plist", "robot.xml");
    armature = cocos2d::extension::CCArmature::create("robot");
    armature->getAnimation()->playByIndex(0);
    armature->setPosition( ccp( VisibleRect::center().x, VisibleRect::bottom().y + armature->getContentSize().height/2 + 30 ) );
    armature->setScale( 1.0f );
    armature->getAnimation()->setSpeedScale(0.5f);
    addChild(armature);
    
    CCParticleSystem *p1 = CCParticleSystemQuad::create("SmallSun.plist");
    CCParticleSystem *p2 = CCParticleSystemQuad::create("SmallSun.plist");
    
    cocos2d::extension::CCBone *bone  = cocos2d::extension::CCBone::create("p1");
    bone->addDisplay(p1, 0);
    bone->changeDisplayByIndex(0, true);
    bone->setIgnoreMovementBoneData(true);
    bone->setZOrder(100);
    bone->setScale(1.2f);
    armature->addBone(bone, "bady-a3");
    
    bone  = cocos2d::extension::CCBone::create("p2");
    bone->addDisplay(p2, 0);
    bone->changeDisplayByIndex(0, true);
    bone->setIgnoreMovementBoneData(true);
    bone->setZOrder(100);
    bone->setScale(1.2f);
    armature->addBone(bone, "bady-a30");

    preloadSprites();
}

void HelloWorld::preloadSprites()
{
    CCSprite* spr = CCSprite::create("greenLaserRay_2.png");
    this->addChild( spr );
    this->removeChild( spr );
}

void HelloWorld::update(float fDelta)
{
    //
    //TODO: Do some game stuff here...
    //
}

void HelloWorld::scrollBackground( float fDelta )
{
    CCPoint backgroundScrollVert = ccp(-1000 * directionMultiplier, 0);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, fDelta)));
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

//
// MY DEMO CODE
//

void HelloWorld::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
//    ++animationID;
//    animationID = animationID % armature->getAnimation()->getMovementCount();
//    armature->getAnimation()->playByIndex(animationID);
    //
    //
    //
    //MJDE CODE
    CCLOG("MJDE CODE Playing animation: %s", armature->getAnimation()->getCurrentMovementID().c_str());
    //MJDE CODE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_OUYA)
#else
    CCDirector* dir = CCDirector::sharedDirector();
    CCPoint touchPoint = pTouch->getLocation();
    if( touchPoint.y <= VisibleRect::top().y && touchPoint.y >= ( dir->getWinSize().height/2 + dir->getWinSize().height/4 ) )
    {
        fireLaser();
//       moveCharacterVertically();
        return true;
    }
    cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification( MOVE_HORIZONTALLY,
                                                                                cocos2d::CCBool::create( touchPoint.x <= dir->getWinSize().width/2 ) );
#endif
    //
    //
    //
    return true;
}

void HelloWorld::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
    stopCharacterMove();
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1, true);
}

//
// GAME CODE
//
void HelloWorld::moveCharacterHorizontally( cocos2d::CCBool* moveLeft )
{
    cocos2d::extension::CCArmatureAnimation* animation = armature->getAnimation();
    if( animation->getCurrentMovementID() == "run" && animation->getIsPlaying() ) return;

    directionMultiplier = ( moveLeft->getValue() ) ? -1.0f : 1.0f;

    armature->stopAllActions();
    armature->setScale( directionMultiplier, 1.0f );
    animation->playByIndex( 1 );
    schedule( schedule_selector(HelloWorld::scrollBackground), 0.01f );
}

void HelloWorld::RunHorizontally( cocos2d::CCBool* moveLeft )
{
    cocos2d::extension::CCArmatureAnimation* animation = armature->getAnimation();
    if( animation->getCurrentMovementID() == "run2" && animation->getIsPlaying() ) return;

    float scaleX = 1.0f;
    directionMultiplier = 5.0f;
    if( moveLeft->getValue() )
    {
        scaleX = -1.0f;
        directionMultiplier = -5.0f;
    }
    armature->stopAllActions();
    armature->setScale( scaleX, 1.0f );
    animation->play( "run2" );
    schedule( schedule_selector( HelloWorld::scrollBackground ), 0.01f );
}

void HelloWorld::moveCharacterVertically()
{
    armature->runAction( CCEaseBackOut::create( CCMoveTo::create( 0.5f,
                                                                 ccp( armature->getPosition().x, VisibleRect::top().y - armature->getContentSize().height + 30 ) ) ) );
    armature->getAnimation()->playByIndex( 6 );
}

void HelloWorld::stopCharacterMove()
{
    cocos2d::extension::CCArmatureAnimation* animation = armature->getAnimation();
    if( animation->getCurrentMovementID() == "stop" && animation->getIsPlaying() ) return;

    armature->stopAllActions();
    armature->setPosition( ccp( VisibleRect::center().x, VisibleRect::bottom().y + armature->getContentSize().height/2 + 30 ) );
    animation->playByIndex( 0 );
    this->unschedule( schedule_selector( HelloWorld::scrollBackground ) );
}

void HelloWorld::fireLaser()
{
    std::string movementID = armature->getAnimation()->getCurrentMovementID();
    if( movementID == "run" || movementID == "run2" ) return;

    CCPoint boneloc = CCPointApplyAffineTransform( armature->getBone("p1")->getPosition(),
                                                  armature->getBone("p1")->nodeToWorldTransform() );
    CCSprite* laser = CCSprite::create( "greenLaserRay_2.png" );
    laser->setPosition( boneloc );
    laser->setScaleX( 0.1f ); laser->setScaleY( 1.0f );
    addChild( laser );

    CCMoveTo* moveTo = CCMoveTo::create( 0.3f,
     ccp( ( directionMultiplier > 0 ) ? VisibleRect::right().x : VisibleRect::left().x, laser->getPosition().y ) );

    CCSequence* seq = CCSequence::create( moveTo, CCRemoveSelf::create(), NULL );
    laser->runAction(CCScaleTo::create( 0.1f, 1.0));
    laser->runAction( seq );
}
