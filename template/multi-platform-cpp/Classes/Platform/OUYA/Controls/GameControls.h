#ifndef __GAME_CONTROLS_H__
#define __GAME_CONTROLS_H__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
#include "VisibleRect.h"
#include "platform/ouya/CCOuyaController.h"

#include "ControlNotifications.h"

#include <jni.h>
#include <initializer_list>

USING_NS_CC;

/**
 * NOTE:
 * 
 * The prefix ai stands for Abbott Industries, the fictional company
 * that mass produces our robots.
 */

class aiGameControls : public cocos2d::CCLayer, public cocos2d::IOuyaControllerListener
{
public:
    /** 
     * Sets the players controller array elements to NULL.
     */
    void ResetPlayersArr();

    /**
     * Checks to see if the the supplied player (controller) has been registered.
     * 
     * @param  controller Controller object.
     * @return bool       True = player is registered. False = player is not registered.
     */
    bool PlayerRegistered( cocos2d::CCOuyaController *controller );

    /**
     * Registers the provided controller and assigns a player number to it.
     * 
     * @param  CCOuyaController Controller object.
     * @return int Returns whatever player number this controller registers as. Player
     *             numbers are assigned in first come, first served basis.
     */
    int RegisterPlayer( cocos2d::CCOuyaController *controller );

    /**
     * Returns the playerID of the given controller.
     */
    int GetPlayerId( cocos2d::CCOuyaController *controller );

    /**
     * Macro that defines a static create method for our class.
     */
    CREATE_FUNC( aiGameControls );

protected:
    /**
     * Checks to see if a specific combination of buttons is being
     * pressed. 
     * 
     * @param  int[]   An array containing enums for controller buttons.
     * @param  int     The length of the data array.
     * @return bool    Returns true if (and only if) the requested button combination is pressed.
     */
    bool IffButtonsPressed( std::initializer_list<int> data );

    /**
     * Checks to see that the passed index is one that we care to test against 
     * when checking the _buttonStates array. Not all indices are relevant to
     * our controller logic. 
     * 
     * @param  int Index to check.
     * @return bool
     */
    bool IsRelevantIndex( int index );

    /**
     * Checks if the controller is in an idle state (no buttons being pushed).
     * @return bool True = is idle. False = buttons are being pushed.
     */
    bool ControllerIsIdle();

    /**
     * Prints the contents of the _buttonStates array to the screen.
     * Use for debugging only!!
     */
    void PrintButtonStates();

private:
    bool _updateAnimationState;

    virtual bool init();  
    // virtual void onExit();
    // virtual void onEnter();
    virtual void update( float fDelta );
    virtual void onControllerKeyUp( int keyCode, cocos2d::CCOuyaController* controller );
    virtual void onControllerKeyDown( int keyCode, cocos2d::CCOuyaController* controller );
    virtual void onControllerLeftStickMotion( float axisXValue, float axisYValue, cocos2d::CCOuyaController* controller );
    virtual void onControllerRightStickMotion( float axisXValue, float axisYValue, cocos2d::CCOuyaController* controller );
};

#endif // __GAME_CONTROLS_H__
