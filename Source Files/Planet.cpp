////////////////////////////////////////////////////////////////////////
// File: Planet.cpp			Class: Planet
// 
// Description:
// Initiation, movement, and rendering of a planet. A sphere is used for
// the model.
//
////////////////////////////////////////////////////////////////////////

#include "Planet.h"

// -- Object Management --

Planet::Planet()
{
	lpMesh = NULL; 
	lpTexture = NULL;

	vPosition = D3DXVECTOR3();
	vPosition.x = 0;
	vPosition.y = 0;
	vPosition.z = 0;
	vRotation = D3DXVECTOR3();
	vRotation.x = 0;
	vRotation.y = 0;
	vRotation.z = 0;
	vScaling = D3DXVECTOR3();
	vScaling.x = 1;
	vScaling.y = 1;
	vScaling.z = 1;
	vRotVel = D3DXVECTOR3();
	vRotVel.x = 0;
	vRotVel.y = 0;
	vRotVel.z = 0;
	fMass = 1;
}

Planet::~Planet()
{
    if( lpMesh  != NULL )
        lpMesh ->Release();
}

// -- Get / Set Methods --

D3DXVECTOR3	Planet::GetPosition()
{
	return vPosition;
}

void Planet::SetPosition(D3DXVECTOR3 pos)
{
	vPosition = pos;
}

D3DXVECTOR3	Planet::GetRotation()
{
	return vRotation;
}

void Planet::SetRotation(D3DXVECTOR3 rot)
{
	vRotation = rot;
}

D3DXVECTOR3	Planet::GetScaling()
{
	return vScaling;
}

void Planet::SetScaling(D3DXVECTOR3 sca)
{
	vScaling = sca;
}

LPDIRECT3DTEXTURE9* Planet::GetTexture()
{
	return &lpTexture;
}

void Planet::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	lpTexture = texture;
}

LPDIRECT3DTEXTURE9* Planet::GetBumpMap()
{
	return &lpBumpMap;
}

void Planet::SetBumpMap(LPDIRECT3DTEXTURE9 bumpMap)
{
	lpBumpMap = bumpMap;
}

float Planet::GetRadius()
{
	return fRadius;
}

void Planet::SetRadius(float radius)
{
	fRadius = radius;
}

float Planet::GetMass()
{
	return fMass;
}

void Planet::SetMass(float mass)
{
	fMass = mass;
}

D3DXVECTOR3 Planet::GetRotVel()
{
	return vRotVel;
}

void Planet::SetRotVel(D3DXVECTOR3 rotVel)
{
	vRotVel = rotVel;
}

// -- Methods --

HRESULT Planet::Initialise(LPDIRECT3DDEVICE9 g_pd3dDevice, float radius)
{
	fRadius = radius;
	fMass = radius;
	int nSlices = 50;
	int nStacks = 50;

	D3DXCreateSphere(g_pd3dDevice, radius, nSlices, nStacks, &lpMesh, NULL);
	LPD3DXMESH pCloneMesh = NULL; 
	HRESULT hr = lpMesh->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, g_pd3dDevice, &pCloneMesh); // Clone the mesh
	lpMesh->Release(); // Release previous mesh

	CUSTOMVERTEX * pVertices;
	if (FAILED(hr = pCloneMesh->LockVertexBuffer(0, (LPVOID*)&pVertices)))
		return hr;

	unsigned int vertexCount = pCloneMesh->GetNumVertices(); // Get vertex count

	D3DXComputeNormals(pCloneMesh, NULL);

	FLOAT twoPI = 2 * D3DX_PI;

	for (unsigned int i=0; i < vertexCount; i++) // Calculate texture coordinates and vertex tangent vector for each vertex
	{
		pVertices->fV = acos(pVertices->z / radius) / D3DX_PI; // calculate texture coordinates

		FLOAT phi = D3DX_PI * pVertices->fV;
		FLOAT ringRadius = radius * sin(phi);
		 
		if (pVertices->y >= 0)
			pVertices->fU = acos(pVertices->x / ringRadius) / twoPI;
		else
			pVertices->fU = (D3DX_PI + acos(pVertices->x / ringRadius)) / twoPI;

		pVertices->fU=asinf(pVertices->norm.x)/D3DX_PI+0.5f;
        pVertices->fV=asinf(pVertices->norm.y)/D3DX_PI+0.5f;

		// Advance pointer to next vertex
		pVertices++;
	}

	pCloneMesh->UnlockVertexBuffer();
	lpMesh = pCloneMesh;

	return S_OK;
}

void Planet::Render(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	lpMesh->DrawSubset( 0 );
}

void Planet::RenderWithEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect)
{
	unsigned int numOfPasses = 1;
	m_pEffect->Begin( &numOfPasses, 0 );	// start an active technique
	// loop through all render passes in technique
	m_pEffect->SetTexture( "g_MeshTexture", lpTexture );
	m_pEffect->SetTexture( "bumpMapTexture", lpBumpMap );
	g_pd3dDevice->SetTexture( 0, lpTexture );
	g_pd3dDevice->SetTexture( 1, lpBumpMap );
	
	for(int passIndex = 0; passIndex < numOfPasses; passIndex++)
	{
		m_pEffect->BeginPass(passIndex);	// begin a pass, within active technique   
		
		lpMesh->DrawSubset( 0 );
		m_pEffect->EndPass(); // signal end of rendering current pass 
	}
	m_pEffect->End();  // signal end of rendering current technique	 
}