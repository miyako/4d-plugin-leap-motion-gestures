#include "leap_listener.h"
	
void MotionListener::onInit(const Leap::Controller& leap)
{

}

void MotionListener::onExit(const Leap::Controller& leap)
{

}

void MotionListener::onConnect(const Leap::Controller& leap)
{
	if(this->getMethodIdForSystemEvent(leap::LEAP_ON_CONNECT)) {
		if(this->lock()) {	
			this->eventSystems.push_back(leap::LEAP_ON_CONNECT);	
			PA_UnfreezeProcess(this->listenerProcessNumber);
		}
	}
}

void MotionListener::onDisconnect(const Leap::Controller& leap)
{
	if(this->getMethodIdForSystemEvent(leap::LEAP_ON_CONNECT)) {	
		if(this->lock()) {	
			this->eventSystems.push_back(leap::LEAP_ON_DISCONNECT);	
			PA_UnfreezeProcess(this->listenerProcessNumber);
		}	
	}	 
}

void MotionListener::onFocusGained(const Leap::Controller& leap)
{
	if(this->getMethodIdForSystemEvent(leap::LEAP_ON_GETTING_FOCUS)) {	
		if(this->lock()) {	
			this->eventSystems.push_back(leap::LEAP_ON_GETTING_FOCUS);	
			PA_UnfreezeProcess(this->listenerProcessNumber);
		}
	}
}

void MotionListener::onFocusLost(const Leap::Controller& leap)
{
	if(this->getMethodIdForSystemEvent(leap::LEAP_ON_LOSING_FOCUS)) {		
		if(this->lock()) {	
			this->eventSystems.push_back(leap::LEAP_ON_LOSING_FOCUS);	
			PA_UnfreezeProcess(this->listenerProcessNumber);
		}	
	}
}

void MotionListener::onFrame(const Leap::Controller& leap)
{
	
	const Leap::Frame frame = leap.frame();
	const Leap::GestureList gestures = frame.gestures();

	if(gestures.count()) {
		
		if(this->lock()) {		
		
			for (int g = 0; g < gestures.count(); ++g) {
				
				Leap::Gesture gesture = gestures[g];
				
				switch (gesture.type()) {
						
					case Leap::Gesture::TYPE_CIRCLE:
					case Leap::Gesture::TYPE_SWIPE:
					case Leap::Gesture::TYPE_KEY_TAP:
					case Leap::Gesture::TYPE_SCREEN_TAP:	
						this->eventGestures.push_back(gesture);
						break;
						
					default:
						break;
				}
			}
		
			PA_UnfreezeProcess(this->listenerProcessNumber);
		}		
	}
}

void MotionListener::setMethodIdForGestureType(unsigned int mId, CUTF8String &name, Leap::Gesture::Type type)
{
	switch (type) {
		case Leap::Gesture::TYPE_CIRCLE:
			this->methodIdForCircle = mId;
			this->methodNameForCircle = name;
			break;
		case Leap::Gesture::TYPE_SWIPE:
			this->methodIdForSwipe = mId;
			this->methodNameForSwipe = name;
			break;			
		case Leap::Gesture::TYPE_KEY_TAP:
			this->methodIdForKeyTap = mId;
			this->methodNameForKeyTap = name;
			break;				
		case Leap::Gesture::TYPE_SCREEN_TAP:
			this->methodIdForScreenTap = mId;
			this->methodNameForScreenTap = name;
			break;				
		default:
			break;
	}
	
}

void MotionListener::setMethodIdForSystemEvent(unsigned int mId, CUTF8String &name, leap::SystemEvent event)
{
	switch (event) {
		case leap::LEAP_ON_GETTING_FOCUS:
			this->methodIdForFocusGained = mId;
			this->methodNameForFocusGained = name;
			break;
		case leap::LEAP_ON_LOSING_FOCUS:
			this->methodIdForFocusLost = mId;
			this->methodNameForFocusLost = name;
			break;	
		case leap::LEAP_ON_CONNECT:
			this->methodIdForConnect = mId;
			this->methodNameForConnect = name;
			break;
		case leap::LEAP_ON_DISCONNECT:
			this->methodIdForDisconnect = mId;
			this->methodNameForDisconnect = name;
			break;				
		default:
			break;
	}	
}

unsigned int MotionListener::getMethodIdForGestureType(Leap::Gesture::Type type)
{
	switch (type) {
		case Leap::Gesture::TYPE_CIRCLE:
			return this->methodIdForCircle;
			break;			
		case Leap::Gesture::TYPE_SWIPE:
			return this->methodIdForSwipe;
			break;
		case Leap::Gesture::TYPE_KEY_TAP:
			return this->methodIdForKeyTap;
			break;			
		case Leap::Gesture::TYPE_SCREEN_TAP:
			return this->methodIdForScreenTap;
			break;				
		default:
			return 0;
			break;
	}
	
}

unsigned int MotionListener::getMethodIdForSystemEvent(leap::SystemEvent event)
{
	switch (event) {
		case leap::LEAP_ON_GETTING_FOCUS:
			return this->methodIdForFocusGained;
			break;			
		case leap::LEAP_ON_LOSING_FOCUS:
			return this->methodIdForFocusLost;
			break;	
		case leap::LEAP_ON_CONNECT:
			return this->methodIdForConnect;
			break;			
		case leap::LEAP_ON_DISCONNECT:
			return this->methodIdForDisconnect;
			break;				
		default:
			return 0;
			break;
	}
	
}

int MotionListener::copyMethodNameForGestureType(Leap::Gesture::Type type, CUTF8String &name)
{
	int ret = 1;
	
	switch (type) {
		case Leap::Gesture::TYPE_CIRCLE:
			name = this->methodIdForCircle;
			break;			
		case Leap::Gesture::TYPE_SWIPE:
			name = this->methodNameForSwipe;
			break;
		case Leap::Gesture::TYPE_KEY_TAP:
			name = this->methodNameForKeyTap;
			break;			
		case Leap::Gesture::TYPE_SCREEN_TAP:
			name = this->methodNameForScreenTap;
			break;				
		default:
			ret = 0;
			break;
	}	
	
	return ret;
}

int MotionListener::copyMethodNameForSystemEvent(leap::SystemEvent event, CUTF8String &name)
{
	int ret = 1;
	
	switch (event) {
		case leap::LEAP_ON_GETTING_FOCUS:
			name = this->methodNameForFocusGained;
			break;			
		case leap::LEAP_ON_LOSING_FOCUS:
			name = this->methodNameForFocusLost;
			break;	
		case leap::LEAP_ON_CONNECT:
			name = this->methodNameForConnect;
			break;			
		case leap::LEAP_ON_DISCONNECT:
			name = this->methodNameForDisconnect;
			break;				
		default:
			ret = 0;
			break;
	}	
	
	return ret;
}

bool MotionListener::shouldTerminate()
{
	return this->listenerShouldTerminate;
}

void MotionListener::terminate()
{
	this->listenerShouldTerminate = true;
	
	this->eventGestures.clear();
	this->eventSystems.clear();	
	
	PA_UnfreezeProcess(this->listenerProcessNumber);	
}

bool MotionListener::lock()
{
	if(!this->listenerIsLocked){
		this->listenerIsLocked = true;
		return true;
	}else{
		return false;
	}
}

void MotionListener::unlock()
{
	this->listenerIsLocked = false;
}

void MotionListener::init()
{
	this->listenerShouldTerminate = false;
	this->listenerIsLocked = false;
}

bool MotionListener::isActive()
{
	return ((this->getMethodIdForGestureType(Leap::Gesture::TYPE_CIRCLE))
			|| (this->getMethodIdForGestureType(Leap::Gesture::TYPE_SWIPE))
			|| (this->getMethodIdForGestureType(Leap::Gesture::TYPE_KEY_TAP))			
			|| (this->getMethodIdForGestureType(Leap::Gesture::TYPE_SCREEN_TAP))
			|| (this->getMethodIdForSystemEvent(leap::LEAP_ON_GETTING_FOCUS))
			|| (this->getMethodIdForSystemEvent(leap::LEAP_ON_LOSING_FOCUS))
			|| (this->getMethodIdForSystemEvent(leap::LEAP_ON_CONNECT))
			|| (this->getMethodIdForSystemEvent(leap::LEAP_ON_DISCONNECT))			
			);	
}

void MotionListener::executeMethod(const Leap::Controller& leap)
{
	for (std::vector<Leap::Gesture>::iterator it = this->eventGestures.begin() ; it != this->eventGestures.end(); ++it) {
			
		Leap::Gesture gesture = *it;
		
		PA_Variable	params[16];
		
		unsigned int paramCount = 0;
		
		switch (gesture.type()) {
				
			case Leap::Gesture::TYPE_SWIPE:
			{
				Leap::SwipeGesture swipe = gesture;
				
				//state
				
				params[0] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[0], swipe.state());
				
				//durationSeconds
				
				params[1] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[1], swipe.durationSeconds());
				
				//speed
				
				params[2] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[2], swipe.speed());
				
				//(Vector) position
				
				params[3] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[3], swipe.position().x);
				
				params[4] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[4], swipe.position().y);
				
				params[5] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[5], swipe.position().z);
				
				//(Vector) startPosition
				
				params[6] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[6], swipe.startPosition().x);
				
				params[7] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[7], swipe.startPosition().y);
				
				params[8] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[8], swipe.startPosition().z);	
				
				//(Vector) direction
				
				params[9] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[9], swipe.direction().pitch());
				
				params[10] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[10], swipe.direction().roll());				
				
				params[11] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[11], swipe.direction().yaw());				
				
				paramCount = 12;
			}
				break;
				
			case Leap::Gesture::TYPE_SCREEN_TAP:	
			{
				Leap::ScreenTapGesture screentap = gesture;
			
				//state
				
				params[0] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[0], screentap.state());
				
				//durationSeconds
				
				params[1] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[1], screentap.durationSeconds());
				
				//(Vector) position
				
				params[2] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[2], screentap.position().x);
				
				params[3] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[3], screentap.position().y);
				
				params[4] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[4], screentap.position().z);
				
				//(Vector) direction
				
				params[5] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[5], screentap.direction().pitch());
				
				params[6] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[6], screentap.direction().roll());				
				
				params[7] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[7], screentap.direction().yaw());
				
				paramCount = 8;
			}	
				break;
				
			case Leap::Gesture::TYPE_KEY_TAP:	
			{
				Leap::KeyTapGesture tap = gesture;
				
				//state
				
				params[0] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[0], tap.state());
				
				//durationSeconds
				
				params[1] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[1], tap.durationSeconds());
				
				//(Vector) position
				
				params[2] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[2], tap.position().x);
				
				params[3] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[3], tap.position().y);
				
				params[4] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[4], tap.position().z);
				
				//(Vector) direction
				
				params[5] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[5], tap.direction().pitch());
				
				params[6] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[6], tap.direction().roll());				
				
				params[7] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[7], tap.direction().yaw());
				
				paramCount = 8;
			}	
				break;	
				
			case Leap::Gesture::TYPE_CIRCLE:	
			{
				Leap::CircleGesture circle = gesture;
				
				//state
				
				params[0] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[0], circle.state());
				
				//durationSeconds
				
				params[1] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[1], circle.durationSeconds());
						
				//direction

				params[2] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[2], circle.pointable().direction().angleTo(circle.normal())  <= Leap::PI/4);
				
				//radius
				
				params[3] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[3], circle.radius());				
				
				//progress
				
				params[4] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[4], circle.progress());
				
				//angle
				
				float sweptAngle = 0;
				if (circle.state() != Leap::Gesture::STATE_START) {
					Leap::CircleGesture previousUpdate = Leap::CircleGesture(leap.frame(1).gesture(circle.id()));
					sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * Leap::PI;
				}
				
				params[5] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[5], sweptAngle);
				
				//(Vector) normal
				
				params[6] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[6], circle.normal().pitch());
				
				params[7] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[7], circle.normal().roll());
				
				params[8] = PA_CreateVariable(eVK_Real);
				PA_SetRealVariable(&params[8], circle.normal().yaw());
				
				paramCount = 9;
			}	
				break;					
				
			default:
				break;
		}
		
		unsigned int mId = this->getMethodIdForGestureType(gesture.type());
		
		if(mId)
			PA_ExecuteMethodByID(mId, params, paramCount);
		
	}
	
	for (std::vector<leap::SystemEvent>::iterator it = this->eventSystems.begin() ; it != this->eventSystems.end(); ++it) {
		
		leap::SystemEvent event = *it;
		
		PA_Variable	params[0];
		
		unsigned int mId = this->getMethodIdForSystemEvent(event);
		
		if(mId)
			PA_ExecuteMethodByID(mId, params, 0);

	}
	
	this->eventGestures.clear();
	this->eventSystems.clear();	
	
	this->unlock();
}

void MotionListener::setProcessNumber(unsigned int pId)
{
	this->listenerProcessNumber = pId;
}