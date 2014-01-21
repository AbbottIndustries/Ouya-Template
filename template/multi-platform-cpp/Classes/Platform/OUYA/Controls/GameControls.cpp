#include "GameControls.h"
#include "support/CCPointExtension.h"
#include "platform/ouya/jni/Java_com_levire_ouyabind_Controller.h"
#include "platform/ouya/CCOuyaController.h"
#include <jni.h>

USING_NS_CC;

#define MAX_PLAYERS       4
#define MAX_BUTTON_STATES 110

cocos2d::CCOuyaController* players[MAX_PLAYERS];

bool _buttonStates[ MAX_BUTTON_STATES ] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

bool aiGameControls::init()
{
    bool success = cocos2d::CCLayer::init();
    CC_ASSERT( success );

    CCOuyaController::addListener(this);
    this->ResetPlayersArr();
    this->scheduleUpdate();
    this->_updateAnimationState = false;
    return true;
}

void aiGameControls::ResetPlayersArr()
{
    for( int i = 0; i < MAX_PLAYERS; i+=1 )
    {
        players[i] = NULL;
    }
}

//
//NOTE: Is an innappropriate to check for face buttons here,
//      as it seems their update can interrupted by other combinations.
//
void aiGameControls::update( float fDelta )
{
    static CCNotificationCenter* notifier = cocos2d::CCNotificationCenter::sharedNotificationCenter();

    if( this->_updateAnimationState )
    {
        if( this->IffButtonsPressed( { BUTTON_DPAD_LEFT, BUTTON_O } ) || 
            this->IffButtonsPressed( { BUTTON_DPAD_RIGHT, BUTTON_O } ) )
        {
            notifier->postNotification( RUN_HORIZONTALLY,
                cocos2d::CCBool::create( _buttonStates[ BUTTON_DPAD_LEFT ] ) );
        }
        else if( this->IffButtonsPressed( { BUTTON_DPAD_LEFT } ) ||
                this->IffButtonsPressed( { BUTTON_DPAD_RIGHT } ) )
        {
            notifier->postNotification( MOVE_HORIZONTALLY,
                            cocos2d::CCBool::create( _buttonStates[ BUTTON_DPAD_LEFT ] ) );
        }
        else //if( this->ControllerIsIdle() )
        {
            notifier->postNotification( STOP_MOVE );
        }
        this->_updateAnimationState = false;
    }
}

bool aiGameControls::IffButtonsPressed( std::initializer_list<int> data )
{
    bool comboFound = true;
    int length = data.size();

    for( int i = 0, j = 0; i < MAX_BUTTON_STATES; i+=1 )
    {
        if( j < length && i == data.begin()[ j ]  )
        {
            comboFound = ( comboFound && _buttonStates[ i ] );
            j+=1;
        }
        else if( _buttonStates[ i ] && this->IsRelevantIndex( i ) ) return false;
    }
    return comboFound;
}

bool aiGameControls::IsRelevantIndex( int index )
{
    const int relevantIndices[ 17 ] = {
        3, 17, 18, 19, 20, 21, 22,
        102, 103, 104, 105, 106, 107
    };
    for( int i = 0; i < 17; i+=1 )
    {
        if( index == relevantIndices[ i ] ) return true;
    }
    return false;
}

bool aiGameControls::ControllerIsIdle()
{
    for( int i = 0; i < MAX_BUTTON_STATES; i+=1 )
    {
        if( _buttonStates[ i ] && this->IsRelevantIndex( i ) ) return false;
    }
    return true;
}

void aiGameControls::onControllerKeyDown( int keyCode, cocos2d::CCOuyaController* controller )
{
    CCAssert( controller, "Expects non-nil controller object." );

    //
    //NOTE: This should actually happen in some pre-game menu thingy.
    //
    if( !this->PlayerRegistered( controller ) )
    {
        int playerID = this->RegisterPlayer( controller );
        CCLOG("Player %d entered the game!", playerID);
    }

    if( !_buttonStates[ keyCode ] )
    {
        _buttonStates[ keyCode ] = true;

        if( keyCode == BUTTON_U )
        {
             cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification( FIRE_LASER );
        }
        else
        {
            this->_updateAnimationState = true;
        }
        CCLOG("%i key was set!!", keyCode);
    }
}

void aiGameControls::onControllerKeyUp( int keyCode, cocos2d::CCOuyaController* controller )
{
    CC_ASSERT( controller );

    /**
     * Uncomment the following to debug _buttonStates.
     */
    // if( keyCode == 82 )
    // {
    //     this->PrintButtonStates();
    // }

    if( _buttonStates[ keyCode ] )
    {
        _buttonStates[ keyCode ] = false;
        this->_updateAnimationState = true;

        CCLOG("%i key was un-set!!", keyCode);
    }
}

void aiGameControls::PrintButtonStates()
{
    for( int i = 0; i < 11; i+=1 )
    {
        CCLOG("%i %i %i %i %i %i %i %i %i %i",
            _buttonStates[i], _buttonStates[i+1], _buttonStates[i+2], _buttonStates[i+3], _buttonStates[i+4],
            _buttonStates[i+5], _buttonStates[i+6], _buttonStates[i+7], _buttonStates[i+8], _buttonStates[i+9]);
    }
}

void aiGameControls::onControllerLeftStickMotion( float axisXValue, float axisYValue, cocos2d::CCOuyaController* controller )
{
    CC_ASSERT( controller );
    // CCLOG("onControllerLEFTStick(%f, %f, %d)", axisXValue, axisYValue, controller);
}

void aiGameControls::onControllerRightStickMotion( float axisXValue, float axisYValue, cocos2d::CCOuyaController* controller )
{
    CC_ASSERT( controller );
    // CCLOG("onControllerRIGHTStick(%f, %f, %d)", axisXValue, axisYValue, controller);
}

bool aiGameControls::PlayerRegistered( cocos2d::CCOuyaController *controller )
{
    CC_ASSERT( controller );

    for( int i = 0; i < MAX_PLAYERS; i+=1 )
    {
        if( players[i] == controller )
        {
            return true;
        }
    }
    return false;
}

int aiGameControls::RegisterPlayer( cocos2d::CCOuyaController *controller )
{
    CC_ASSERT( controller );
    CC_ASSERT( !this->PlayerRegistered( controller ) );

    for( int i = 0; i < MAX_PLAYERS; i+=1 ) 
    {
        if (players[i] == NULL)
        {
            players[i] = controller;
            return i;
        }
    }

    CC_ASSERT( false );
}

int aiGameControls::GetPlayerId( cocos2d::CCOuyaController *controller )
{
    CC_ASSERT( controller );

    for( int i = 0; i < MAX_PLAYERS; i+=1 )
    {
        if( players[i] == controller )
        {
            return i;
        }
    }
    return -1;
}

// void aiGameControls::onEnter()
// {
//     CCLayer::onEnter();
//     //
//     //TODO: Evaluate whether or not overriding this method is necessary here...
//     //
// }

// void aiGameControls::onExit()
// {
//     CCLayer::onExit();
//     //
//     //TODO: Evaluate whether or not overriding this method is necessary here...
//     //
// }
