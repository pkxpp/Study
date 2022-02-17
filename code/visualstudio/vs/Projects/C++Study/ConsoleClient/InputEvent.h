#pragma once

enum enuInputEvent
{
	enuInputEvent_None = 0,

	enuInputEvent_Idle,
	enuInputEvent_Forward,
	enuInputEvent_Turn,
	enuInputEvent_Run,
	enuInputEvent_Interact,
	enuInputEvent_Jump,
	enuInputEvent_LightAttack,
	enuInputEvent_HardAttack,
	enuInputEvent_Weapon,

	/* use for camera */
	enuInputEvent_CameraToggleControl,
	enuInputEvent_CameraZoom,
	enuInputEvent_CameraBeginDrag,
	enuInputEvent_CameraEndDrag,
	enuInputEvent_CameraClearControl,
	enuInputEvent_CameraResetControl,
	enuInputEvent_CameraSwitchMode,
	enuInputEvent_CameraSwitchControlMode,
	enuInputEvent_CameraStickMove,

	enuInputEvent_Count
};

struct InputEventHeader
{
	uint uSize;
	enuInputEvent eEvent;
};

struct InputEventIdle : public InputEventHeader
{

};

struct InputEventInteract : public InputEventHeader
{
	BOOL bActivate;
};

struct InputEventRun : public InputEventHeader
{
	BOOL bActivate;
};

struct InputEventLightAttack : public InputEventHeader
{
};

struct InputEventHardAttack : public InputEventHeader
{
};

struct InputEventWeapon : public InputEventHeader
{
};


struct InputEventCameraToggleControl : public InputEventHeader
{
	BOOL bPress;
    int eControlState;
};

struct InputEventCameraZoom : public InputEventHeader
{
	float fScale;
};

struct InputEventCameraBeginDrag : public InputEventHeader
{

};

struct InputEventCameraEndDrag : public InputEventHeader
{

};

struct InputEventCameraClearControl : public InputEventHeader
{

};

struct InputEventCameraResetControl : public InputEventHeader
{

};

struct InputEventCameraSwitchMode : public InputEventHeader
{

};

struct InputEventCameraSwitchControlMode : public InputEventHeader
{

};

struct InputEventCameraStickMove : public InputEventHeader
{
	float fX;
	float fY;
};