//
//  GameControllerEvent.h
//  Super Hexagon
//
//  Created by Daniel Rosser on 6/11/2015.
//
//

#ifndef GameControllerEvent_h
#define GameControllerEvent_h

class GameControllerEvent {
public:
    enum Type {
        none = 0,
        leftButton,
        rightButton,
        upButton,
        downButton,
        buttonX,
        buttonA,
        buttonB,
        buttonY,
        leftThumbStickButton,
        rightThumbStickButton,
        l1Button,
        l2Button,
        r1Button,
        r2Button,
        infoButton,
        leftThumbstick,
        rightThumbstick,
        connected,
        disconnected
    };
    
    GameControllerEvent() : buttonType(none), value(0), isPressed(false), xValue(0), controllerID(0) {
        
    }
    
    GameControllerEvent(Type type, float theValue, bool pressed): buttonType(type), value(theValue), isPressed(pressed), xValue(0) , controllerID(0) {
        
    }
    
    GameControllerEvent(Type type, float theValue, bool pressed, float xxValue): buttonType(type), value(theValue), isPressed(pressed), xValue(xxValue) , controllerID(0) {
        
    }
    
    Type buttonType;
    float value;
    float xValue;
    bool isPressed;
    int controllerID;
    
};


#endif /* GameControllerEvent_h */
