////////////////////////////////////////////////////////////////////////
// File: Quad.cpp			Class: Quad
// 
// Description:
// A 3D textured quad.
//
////////////////////////////////////////////////////////////////////////

#include "Quad.h"
#include <d3d9.h>
#include <d3dx9.h>
#include<fstream>
#include<windows.h>

// -- Object Management --

Quad::Quad()
{
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
}

// -- Get / Set Methods --

D3DXVECTOR3	Quad::GetPosition()
{
	return vPosition;
}

void Quad::SetPosition(D3DXVECTOR3 pos)
{
	vPosition = pos;
}

D3DXVECTOR3	Quad::GetRotation()
{
	return vRotation;
}

void Quad::SetRotation(D3DXVECTOR3 rot)
{
	vRotation = rot;
}

D3DXVECTOR3	Quad::GetScaling()
{
	return vScaling;
}

void Quad::SetScaling(D3DXVECTOR3 sca)
{
	vScaling = sca;
}

LPDIRECT3DTEXTURE9* Quad::GetTexture()
{
	return &lpTexture;
}

void Quad::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	lpTexture = texture;
}

// -- Methods --

void Quad::CreateQuad(LPDIRECT3DDEVICE9 p_dx_Device, D3DXVECTOR3 TL, D3DXVECTOR3 TR, D3DXVECTOR3 BL, D3DXVECTOR3 BR)
{
	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	CUSTOMVERTEX * cv_Vertices = new CUSTOMVERTEX[4];

	cv_Vertices[0].x = TL.x;
	cv_Vertices[0].y = TL.y;
	cv_Vertices[0].z = TL.z;
	cv_Vertices[0].fU = 0;
	cv_Vertices[0].fV = 0;

	cv_Vertices[1].x = TR.x;
	cv_Vertices[1].y = TR.y;
	cv_Vertices[1].z = TR.z;
	cv_Vertices[1].fU = 1;
	cv_Vertices[1].fV = 0;

	cv_Vertices[2].x = BL.x;
	cv_Vertices[2].y = BL.y;
	cv_Vertices[2].z = BL.z;
	cv_Vertices[2].fU = 0;
	cv_Vertices[2].fV = 1;

	cv_Vertices[3].x = BR.x;
	cv_Vertices[3].y = BR.y;
	cv_Vertices[3].z = BR.z;
	cv_Vertices[3].fU = 1;
	cv_Vertices[3].fV = 1;

	if (FAILED(p_dx_Device->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
	{
		MessageBox(NULL,"Error while creating VertexBuffer","FillVertices()",MB_OK);
	}

	VOID* p_Vertices;
	if (FAILED(p_dx_VertexBuffer->Lock(0, 4*sizeof(CUSTOMVERTEX), (void**)&p_Vertices, 0)))
	{
		MessageBox(NULL,"Error trying to lock","FillVertices()",MB_OK);
	}else{
		memcpy(p_Vertices, cv_Vertices, 4*sizeof(CUSTOMVERTEX));
		p_dx_VertexBuffer->Unlock();
	}

	lpQuadVB = p_dx_VertexBuffer;

	LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
	short * s_Indices = new short[6];

	s_Indices[0]=0;
	s_Indices[1]=1;
	s_Indices[2]=2;
	s_Indices[3]=1;
	s_Indices[4]=2;
	s_Indices[5]=3;

	if (FAILED(p_dx_Device->CreateIndexBuffer(6*sizeof(short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&p_dx_IndexBuffer,NULL)))
	{
		MessageBox(NULL,"Error while creating IndexBuffer","FillIndices()",MB_OK);
	}

	VOID* p_Indices;
	if (FAILED(p_dx_IndexBuffer->Lock(0, 6*sizeof(short), (void**)&p_Indices, 0)))
	{
		MessageBox(NULL,"Error trying to lock","FillIndices()",MB_OK);
	}else{
		memcpy(p_Indices, s_Indices, 6*sizeof(short));
		p_dx_IndexBuffer->Unlock();
	}

	lpQuadIB = p_dx_IndexBuffer;
}

void Quad::CreateQuad(LPDIRECT3DDEVICE9 p_dx_Device, CUSTOMVERTEX TL, CUSTOMVERTEX TR, CUSTOMVERTEX BL, CUSTOMVERTEX BR)
{
	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	CUSTOMVERTEX * cv_Vertices = new CUSTOMVERTEX[4];

	cv_Vertices[0] = TL;
	cv_Vertices[1] = TR;
	cv_Vertices[2] = BL;
	cv_Vertices[3] = BR;

	if (FAILED(p_dx_Device->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
	{
		MessageBox(NULL,"Error while creating VertexBuffer","FillVertices()",MB_OK);
	}

	VOID* p_Vertices;
	if (FAILED(p_dx_VertexBuffer->Lock(0, 4*sizeof(CUSTOMVERTEX), (void**)&p_Vertices, 0)))
	{
		MessageBox(NULL,"Error trying to lock","FillVertices()",MB_OK);
	}else{
		memcpy(p_Vertices, cv_Vertices, 4*sizeof(CUSTOMVERTEX));
		p_dx_VertexBuffer->Unlock();
	}

	lpQuadVB = p_dx_VertexBuffer;

	LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
	short * s_Indices = new short[6];

	s_Indices[0]=0;
	s_Indices[1]=1;
	s_Indices[2]=2;
	s_Indices[3]=1;
	s_Indices[4]=2;
	s_Indices[5]=3;

	if (FAILED(p_dx_Device->CreateIndexBuffer(6*sizeof(short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&p_dx_IndexBuffer,NULL)))
	{
		MessageBox(NULL,"Error while creating IndexBuffer","FillIndices()",MB_OK);
	}

	VOID* p_Indices;
	if (FAILED(p_dx_IndexBuffer->Lock(0, 6*sizeof(short), (void**)&p_Indices, 0)))
	{
		MessageBox(NULL,"Error trying to lock","FillIndices()",MB_OK);
	}else{
		memcpy(p_Indices, s_Indices, 6*sizeof(short));
		p_dx_IndexBuffer->Unlock();
	}

	lpQuadIB = p_dx_IndexBuffer;
}

void Quad::Render(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect)
{ 
	unsigned int numOfPasses = 1;
	m_pEffect->Begin( &numOfPasses, 0 );	// start an active technique
	// loop through all render passes in technique
	for(int passIndex = 0; passIndex < numOfPasses; passIndex++)
	{
		m_pEffect->BeginPass(passIndex);	// begin a pass, within active technique   
		
		m_pEffect->SetTexture( "g_MeshTexture", lpTexture );

		g_pd3dDevice->SetStreamSource(0, lpQuadVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetTexture(0,lpTexture);
		g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		g_pd3dDevice->SetIndices(lpQuadIB);
		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

		m_pEffect->EndPass(); // signal end of rendering current pass 
	}
	m_pEffect->End();  // signal end of rendering current technique	 
}