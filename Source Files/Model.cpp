////////////////////////////////////////////////////////////////////////
// File: Model.cpp			Class: Model
// 
// Description:
// Initiation, movement, and rendering of a model.
//
////////////////////////////////////////////////////////////////////////

#include "Model.h"

// -- Object Management --

Model::Model()
{
	lpMesh = NULL; 
	dNumMaterials = 0; 
	pMeshMaterials = NULL; 
	lppMeshTextures = NULL;
	vPosition = D3DXVECTOR3();
	vPosition.x = 0;
	vPosition.y = 0;
	vPosition.z = 0;
	vVelocity = D3DXVECTOR3();
	vVelocity.x = 0;
	vVelocity.y = 0;
	vVelocity.z = 0;
	vRotation = D3DXVECTOR3();
	vRotation.x = 0;
	vRotation.y = 0;
	vRotation.z = 0;
	vScaling = D3DXVECTOR3();
	vScaling.x = 1;
	vScaling.y = 1;
	vScaling.z = 1;
	fMass = 0.1f;
}

Model::~Model()
{
	if( pMeshMaterials  != NULL )
		delete[] pMeshMaterials;

	if( lppMeshTextures )
    {
		for( DWORD i = 0; i < dNumMaterials; i++ )
        {
            if( lppMeshTextures [i] )
                lppMeshTextures [i]->Release();
        }
        delete[] lppMeshTextures ;
    }
    if( lpMesh  != NULL )
        lpMesh ->Release();
}

// -- Get / Set Methods --

D3DXVECTOR3	Model::GetPosition()
{
	return vPosition;
}

void Model::SetPosition(D3DXVECTOR3 pos)
{
	vPosition = pos;
}

D3DXVECTOR3	Model::GetRotation()
{
	return vRotation;
}

void Model::SetRotation(D3DXVECTOR3 rot)
{
	vRotation = rot;
}

D3DXVECTOR3	Model::GetScaling()
{
	return vScaling;
}

void Model::SetScaling(D3DXVECTOR3 sca)
{
	vScaling = sca;
}

float Model::GetRadius()
{
	return fRadius;
}

void Model::SetRadius(float radius)
{
	fRadius = radius;
}

float Model::GetMass()
{
	return fMass;
}

void Model::SetMass(float mass)
{
	fMass = mass;
}

D3DXVECTOR3	Model::GetVelocity()
{
	return vVelocity;
}

void Model::SetVelocity(D3DXVECTOR3 vel)
{
	vVelocity = vel;
}

// -- Methods --

HRESULT Model::Initialise(LPDIRECT3DDEVICE9 g_pd3dDevice, LPCTSTR pFilename)
{
	LPD3DXBUFFER pMaterialBuffer;
	HRESULT hr = D3DXLoadMeshFromX( pFilename, D3DXMESH_MANAGED, g_pd3dDevice, NULL, &pMaterialBuffer, NULL, &dNumMaterials, &lpMesh ); 
	if ( FAILED( hr ) ) 
	{
		MessageBox( NULL, "Could not find mesh", "Meshes.exe", MB_OK );
		return FALSE;
	}

	// Store material and texture information 
	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dNumMaterials]; 
	lppMeshTextures = new LPDIRECT3DTEXTURE9[dNumMaterials];

	// Copy the materials and textures from the buffer to the member arrays 
	for ( DWORD i = 0; i < dNumMaterials; i++ ) 
	{
		pMeshMaterials[i] = pMaterials[i].MatD3D; 
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse; 
		// Create the texture if it exists 
		lppMeshTextures[i] = NULL; 
		if( pMaterials[i].pTextureFilename ) 
		{
			if ( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, pMaterials[i].pTextureFilename, &lppMeshTextures[i] ) ) ) 
			{
				MessageBox( NULL, "Could not find mesh texture", "Meshes.exe", MB_OK );
				return FALSE;
			}
		}
	}

	pMaterialBuffer->Release();

	return S_OK;
}

void Model::Render(LPDIRECT3DDEVICE9 g_pd3dDevice)
{ 
	for ( DWORD i = 0; i < dNumMaterials; i++ ) 
	{
		g_pd3dDevice->SetMaterial( &pMeshMaterials[i] ); 
		g_pd3dDevice->SetTexture( 0, lppMeshTextures[i] ); 
		lpMesh->DrawSubset( i );
	}
}

void Model::RenderWithEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect)
{
	unsigned int numOfPasses;
	m_pEffect->Begin( &numOfPasses, 0 );	// start an active technique
	
	// loop through all render passes in technique
	for(int passIndex = 0; passIndex < numOfPasses; passIndex++)
	{
		m_pEffect->BeginPass(passIndex);	// begin a pass, within active technique   
		
		for ( DWORD i = 0; i < dNumMaterials; i++ ) 
		{
			g_pd3dDevice->SetMaterial( &pMeshMaterials[i] ); 
			g_pd3dDevice->SetTexture( 0, lppMeshTextures[i] ); 
			lpMesh->DrawSubset( i );
		}

		m_pEffect->EndPass(); // signal end of rendering current pass 
	}
	m_pEffect->End();  // signal end of rendering current technique
}