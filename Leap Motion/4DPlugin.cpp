/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : Leap Motion
 #	author : miyako
 #	2013/10/10
 #
 # --------------------------------------------------------------------------------*/

#include "4DPlugin.h"

namespace leap
{
	MotionListener listener;
	Leap::Controller controller;
}

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
		case kDeinitPlugin:
		case kServerDeinitPlugin:
			listenerLoopFinish();
			break;				

		case kInitPlugin:
		case kServerInitPlugin:
			leap::listener.init();
			break;	
			
			// --- Gestures
			
		case 1 :
			LEAP_Set_listener(pResult, pParams);
			break;
			
		case 2 :
			LEAP_Get_listener(pResult, pParams);
			break;
			
			// --- Settings
			
		case 3 :
			LEAP_Set_policy(pResult, pParams);
			break;
			
		case 4 :
			LEAP_Get_policy(pResult, pParams);
			break;
			
		case 5 :
			LEAP_Get_parameter(pResult, pParams);
			break;
			
		case 6 :
			LEAP_Set_parameter(pResult, pParams);
			break;
			
			// --- Device
			
		case 7 :
			LEAP_Get_device_name(pResult, pParams);
			break;
			
		case 8 :
			LEAP_Get_device_vertical(pResult, pParams);
			break;
			
		case 9 :
			LEAP_Is_application_frontmost(pResult, pParams);
			break;
			
		case 10 :
			LEAP_Get_device_range(pResult, pParams);
			break;
			
		case 11 :
			LEAP_Get_device_horizontal(pResult, pParams);
			break;
			
		case 12 :
			LEAP_Is_device_connected(pResult, pParams);
			break;
	}
}

#pragma mark -

void listenerLoopFinish(){
	
	leap::listener.terminate();	

}

void listenerBegin(){

	CUTF8String processName = CUTF8String((const uint8_t *)"$Leap Motion Listener Process");
	
	C_TEXT name;
	name.copyUTF8String(&processName);
	
	unsigned int pId = PA_NewProcess((void *)listenerLoop, -512*1024, (PA_Unichar *)name.getUTF16StringPtr());
	
	PA_FreezeProcess(pId);	
	
	leap::listener.setProcessNumber(pId);
	
	leap::controller.addListener(leap::listener);
}

void listenerLoop(){
	
	unsigned int pId = PA_GetCurrentProcessNumber();
	
	char done = 0;
	
	while (!done)
	{ 
		PA_YieldAbsolute();
		done = (PA_IsProcessDying()) || (leap::listener.shouldTerminate());
		
		if (!done){		
			
			leap::listener.executeMethod(leap::controller);
			
			PA_FreezeProcess(pId);
			
		}
	}
	
	leap::controller.removeListener(leap::listener);
	
	PA_KillProcess();
	
	leap::listener.init();
}

#pragma mark -

#pragma mark Listener

// ----------------------------------- Gestures -----------------------------------

void LEAP_Set_listener(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	int success = 0;
	
	Leap::Gesture::Type type = (Leap::Gesture::Type)Param1.getIntValue(); 
	
	CUTF8String name;
	Param2.copyUTF8String(&name);
	
	if(!name.size()) {
		
		switch (type) {
			//system events	
			case leap::LEAP_ON_GETTING_FOCUS:
			case leap::LEAP_ON_LOSING_FOCUS:
			case leap::LEAP_ON_CONNECT:
			case leap::LEAP_ON_DISCONNECT:
			{
				leap::SystemEvent event = (leap::SystemEvent)type;
				leap::listener.setMethodIdForSystemEvent(0, name, event);
			}
				break;
			//gesture	
			case Leap::Gesture::TYPE_CIRCLE:
			case Leap::Gesture::TYPE_SWIPE:
			case Leap::Gesture::TYPE_KEY_TAP:
			case Leap::Gesture::TYPE_SCREEN_TAP:	
				leap::listener.setMethodIdForGestureType(0, name, type);
				leap::controller.enableGesture(type, false);
				break;
			default:
				break;	
		}
		
		if(!leap::listener.isActive()) {
			
			listenerLoopFinish();
		} 		
		
		success = 1;
		
	}else{		
		
		unsigned int mId = PA_GetMethodID((PA_Unichar *)Param2.getUTF16StringPtr());
		
		if(mId){
			
			bool isActive = leap::listener.isActive();
			
			switch (type) {
					//system	
				case leap::LEAP_ON_GETTING_FOCUS:
				case leap::LEAP_ON_LOSING_FOCUS:
				case leap::LEAP_ON_CONNECT:
				case leap::LEAP_ON_DISCONNECT:
				{
					leap::SystemEvent event = (leap::SystemEvent)type;
					leap::listener.setMethodIdForSystemEvent(mId, name, event);
				}
					break;
					//gesture	
				case Leap::Gesture::TYPE_CIRCLE:
				case Leap::Gesture::TYPE_SWIPE:
				case Leap::Gesture::TYPE_KEY_TAP:
				case Leap::Gesture::TYPE_SCREEN_TAP:
					leap::listener.setMethodIdForGestureType(mId, name, type);
					leap::controller.enableGesture(type, true);
					break;
				default:
					break;					
			}
			
			if(!isActive) 
				listenerBegin();
			
			success = 1;
			
		}
		
	}
	
	returnValue.setIntValue(success);
	returnValue.setReturn(pResult);
}

void LEAP_Get_listener(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	Leap::Gesture::Type type = (Leap::Gesture::Type)Param1.getIntValue(); 
	
	CUTF8String name;
	
	switch (type) {
			//system	
		case leap::LEAP_ON_GETTING_FOCUS:
		case leap::LEAP_ON_LOSING_FOCUS:
		case leap::LEAP_ON_CONNECT:
		case leap::LEAP_ON_DISCONNECT:			
		{
			leap::SystemEvent event = (leap::SystemEvent)type;
			returnValue.setIntValue(leap::listener.copyMethodNameForSystemEvent(event, name));	
			Param2.setUTF8String(&name);
		}
			break;
			//gesture	
		case Leap::Gesture::TYPE_CIRCLE:
		case Leap::Gesture::TYPE_SWIPE:
		case Leap::Gesture::TYPE_KEY_TAP:
		case Leap::Gesture::TYPE_SCREEN_TAP:
			returnValue.setIntValue(leap::listener.copyMethodNameForGestureType(type, name));	
			Param2.setUTF8String(&name);
			break;
		default:
			break;				
	}

	Param2.toParamAtIndex(pParams, 2);
	returnValue.setReturn(pResult);
}

// ----------------------------------- Settings -----------------------------------

#pragma mark Policy

void LEAP_Set_policy(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	Leap::Controller::PolicyFlag policy = (Leap::Controller::PolicyFlag)Param1.getIntValue();
	leap::controller.setPolicyFlags(policy); 
	
	/*
	 Policy changes are completed asynchronously 
	 and, because they are subject to user approval, 
	 may not complete successfully.
	 This function can be called before the Controller object is connected, 
	 but the request will be sent to the Leap Motion software after the Controller connects.
	 */
	
	returnValue.setIntValue(1);//always success
	returnValue.setReturn(pResult);
}

void LEAP_Get_policy(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
	
	returnValue.setIntValue(leap::controller.policyFlags());	
	returnValue.setReturn(pResult);
}

#pragma mark Config

bool getKeyStringForKey(leap::ConfigKey key, std::string &keyString)
{
	switch (key) {
		case leap::LEAP_CIRCLE_MIN_RADIUS:
			keyString = "Gesture.Circle.MinRadius";
			break;
		case leap::LEAP_CIRCLE_MIN_ARC:
			keyString = "Gesture.Circle.MinArc";
			break;
		case leap::LEAP_SWIPE_MIN_LENGTH:
			keyString = "Gesture.Swipe.MinLength";
			break;	
		case leap::LEAP_SWIPE_MIN_VELOCITY:
			keyString = "Gesture.Swipe.MinVelocity";
			break;	
		case leap::LEAP_KEYTAP_MIN_VELOCITY:
			keyString = "Gesture.KeyTap.MinDownVelocity";
			break;		
		case leap::LEAP_KEYTAP_HISTORY:
			keyString = "Gesture.KeyTap.HistorySeconds";
			break;	
		case leap::LEAP_KEYTAP_MIN_DISTANCE:
			keyString = "Gesture.KeyTap.MinDistance";
			break;	
		case leap::LEAP_SCREENTAP_MIN_VELOCITY:
			keyString = "Gesture.ScreenTap.MinForwardVelocity";
			break;	
		case leap::LEAP_SCREENTAP_HISTORY:
			keyString = "Gesture.ScreenTap.HistorySeconds";
			break;
		case leap::LEAP_SCREENTAP_MIN_DISTANCE:
			keyString = "Gesture.ScreenTap.MinDistance";
			break;			
		default:
			keyString = "";
			break;
	}
	
	return keyString.size();
}

void LEAP_Get_parameter(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_REAL Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	leap::ConfigKey key = (leap::ConfigKey)Param1.getIntValue();
	std::string keyString;
	
	if (getKeyStringForKey(key, keyString)){
		
		Param2.setDoubleValue(leap::controller.config().getFloat(keyString));
		returnValue.setIntValue(1);
		
	}
	
	Param2.toParamAtIndex(pParams, 2);
	returnValue.setReturn(pResult);
}

void LEAP_Set_parameter(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_REAL Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	leap::ConfigKey key = (leap::ConfigKey)Param1.getIntValue();
	std::string keyString;

	if (getKeyStringForKey(key, keyString)){
	
		leap::controller.config().setFloat(keyString, Param2.getDoubleValue());
		returnValue.setIntValue(leap::controller.config().save());
	
	}
		
	returnValue.setReturn(pResult);
}

#pragma mark Device

// ------------------------------------ Device ------------------------------------

void LEAP_Get_device_name(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT returnValue;
	
	Leap::DeviceList devices = leap::controller.devices();
	
	if(!devices.isEmpty()) {
		
		std::string info = devices[0].toString();
		returnValue.setUTF8String((const uint8_t*)info.c_str(), info.size());
		
	}
	
	returnValue.setReturn(pResult);
}

void LEAP_Get_device_vertical(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_REAL returnValue;
	
	Leap::DeviceList devices = leap::controller.devices();	
	
	if(!devices.isEmpty())
		returnValue.setDoubleValue(devices[0].verticalViewAngle());
	
	returnValue.setReturn(pResult);
}

void LEAP_Get_device_range(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_REAL returnValue;
	
	Leap::DeviceList devices = leap::controller.devices();	
	
	if(!devices.isEmpty())
		returnValue.setDoubleValue(devices[0].range());
	
	returnValue.setReturn(pResult);
}

void LEAP_Get_device_horizontal(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_REAL returnValue;
	
	Leap::DeviceList devices = leap::controller.devices();
	
	if(!devices.isEmpty())
		returnValue.setDoubleValue(devices[0].horizontalViewAngle());
	
	returnValue.setReturn(pResult);
}

void LEAP_Is_device_connected(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
	
	returnValue.setIntValue(leap::controller.isConnected());
	
	returnValue.setReturn(pResult);
}

void LEAP_Is_application_frontmost(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
	
	returnValue.setIntValue(leap::controller.hasFocus());
	
	returnValue.setReturn(pResult);
}
