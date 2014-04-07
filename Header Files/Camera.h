////////////////////////////////////////////////////////////////////////
// File: Camera.h			Class: Camera
// 
// Description:
// This controls the view of the game, from the players perspective.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_CAMERA
#define HG_CAMERA

#include <d3dx9.h>

class Camera
{
private:
	// -- Variables --
	D3DXVECTOR3		vEyePt;
	D3DXVECTOR3		vLookAt;
	D3DXVECTOR3		vUpVec;
	float			fRotation;

public:
	// -- Object Management --
	Camera				();
	~Camera				();

	// -- Get / Set Methods --
	D3DXVECTOR3 GetEyePt	();
	void		SetEyePt	(D3DXVECTOR3 eyePt);
	D3DXVECTOR3 GetLookAt	();
	void		SetLookAt	(D3DXVECTOR3 lookAt);
	D3DXVECTOR3 GetUpVec	();
	void		SetUpVec	(D3DXVECTOR3 upVec);

	// -- Methods --
	void SetPosition	(D3DXVECTOR3 eyePt, D3DXVECTOR3 lookAt);
	void Move			(D3DXVECTOR3 relPos);
	void Rotate			(float rot);
};

#endif // HG_CAMERA