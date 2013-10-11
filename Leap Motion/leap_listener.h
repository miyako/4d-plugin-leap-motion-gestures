#include "4DPluginAPI.h"

#include "Leap.h"

namespace leap
{
	typedef enum
	{
		LEAP_CIRCLE_MIN_RADIUS,
		LEAP_CIRCLE_MIN_ARC,
		LEAP_SWIPE_MIN_LENGTH,
		LEAP_SWIPE_MIN_VELOCITY,
		LEAP_KEYTAP_MIN_VELOCITY,
		LEAP_KEYTAP_HISTORY,
		LEAP_KEYTAP_MIN_DISTANCE,
		LEAP_SCREENTAP_MIN_VELOCITY,
		LEAP_SCREENTAP_HISTORY,
		LEAP_SCREENTAP_MIN_DISTANCE
	}ConfigKey;
	
	typedef enum
	{
		LEAP_ON_GETTING_FOCUS = 101,
		LEAP_ON_LOSING_FOCUS,
		LEAP_ON_CONNECT,
		LEAP_ON_DISCONNECT		
	}SystemEvent;	
}

class MotionListener : public Leap::Listener {

private:
	
	unsigned int methodIdForSwipe;
	unsigned int methodIdForCircle;
	unsigned int methodIdForScreenTap;
	unsigned int methodIdForKeyTap;

	unsigned int methodIdForFocusGained;
	unsigned int methodIdForFocusLost;
	unsigned int methodIdForConnect;
	unsigned int methodIdForDisconnect;	
	
	CUTF8String methodNameForSwipe;
	CUTF8String methodNameForCircle;
	CUTF8String methodNameForScreenTap;
	CUTF8String methodNameForKeyTap;
	
	CUTF8String methodNameForFocusGained;
	CUTF8String methodNameForFocusLost;
	CUTF8String methodNameForConnect;
	CUTF8String methodNameForDisconnect;	
	
	std::vector<Leap::Gesture> eventGestures;
	std::vector<leap::SystemEvent> eventSystems;
	
	unsigned int listenerProcessNumber;
	
	bool listenerShouldTerminate;
	bool listenerIsLocked;
	
public:
	
	void onInit(const Leap::Controller& leap);
	void onExit(const Leap::Controller& leap);	
	
	void onConnect(const Leap::Controller& leap);
	void onDisconnect(const Leap::Controller& leap);
	
	void onFocusGained(const Leap::Controller& leap);	
	void onFocusLost(const Leap::Controller& leap);	
	
	void onFrame(const Leap::Controller& leap);	
	
	void setMethodIdForGestureType(unsigned int mId, CUTF8String &name, Leap::Gesture::Type type);
	void setMethodIdForSystemEvent(unsigned int mId, CUTF8String &name, leap::SystemEvent event);
	
	unsigned int getMethodIdForGestureType(Leap::Gesture::Type type);
	unsigned int getMethodIdForSystemEvent(leap::SystemEvent event);
	
	int copyMethodNameForGestureType(Leap::Gesture::Type type, CUTF8String &name);	
	int copyMethodNameForSystemEvent(leap::SystemEvent event, CUTF8String &name);	
	
	bool shouldTerminate();
	void terminate();	
	
	bool lock();
	void unlock();
	
	void init();	
	
	bool isActive();	
	
	void executeMethod(const Leap::Controller& leap);
	
	void setProcessNumber(unsigned int pId);	
};