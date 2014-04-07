////////////////////////////////////////////////////////////////////////
// File: Camera.cpp			Class: Camera
// 
// Description:
// This controls the view of the game, from the players perspective.
//
////////////////////////////////////////////////////////////////////////

#include "Camera.h"

// -- Object Management --

Camera::Camera()
{
	vEyePt	= D3DXVECTOR3( 0.0f, 0.0f,-40.0f );
	vLookAt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vUpVec	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	fRotation = 0.0f;
}

Camera::~Camera()
{
}

// -- Get / Set Methods --

D3DXVECTOR3 Camera::GetEyePt()
{
	return vEyePt;
}

void Camera::SetEyePt(D3DXVECTOR3 eyePt)
{
	vEyePt = eyePt;
}

D3DXVECTOR3 Camera::GetLookAt()
{
	return vLookAt;
}

void Camera::SetLookAt(D3DXVECTOR3 lookAt)
{
	vLookAt = lookAt;
}

D3DXVECTOR3 Camera::GetUpVec()
{
	return vUpVec;
}

void Camera::SetUpVec(D3DXVECTOR3 upVec)
{
	vUpVec = upVec;
}

// -- Methods --

void Camera::SetPosition(D3DXVECTOR3 eyePt, D3DXVECTOR3 lookAt)
{
    vEyePt	= eyePt;
	vLookAt	= lookAt;
}

void Camera::Move(D3DXVECTOR3 relPos)
{
    vEyePt	+= relPos;
	vLookAt += relPos;
}

void Camera::Rotate(float rot)
{
	fRotation += rot;

	D3DXVECTOR3 newlookat = D3DXVECTOR3();

	newlookat.x = cos(fRotation);
	newlookat.z = sin(fRotation);

	newlookat.x *= 20;
	newlookat.x += vEyePt.x;
	newlookat.z *= 20;
	newlookat.z += vEyePt.z;

	vLookAt.x = newlookat.x;
	vLookAt.z = newlookat.z;
}