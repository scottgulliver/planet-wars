//******************************************
//**                                      **
//**         Structs & Global Data        **
//**                                      **
//******************************************

// Vertex shader parameters (set in C++ application code) 
float4x4 wMat;
float4x4 vMat;
float4x4 pMat;
float4 vLightPos;
float4 vEyePos;
Texture2D modelTexture : register( t0 ); 
Texture2D bumpMapTexture : register( t1 ); 
Texture2D atmosMapTexture : register( t2 );
float shininess;
float transparency;
float4 forcePos;
bool forceAffected;

struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
};

struct VertexShaderOutput
{
	float4 pos : POSITION;
	float4 norm : TEXCOORD; 
};

struct VertexShaderInputPhong
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
};

struct PixelShaderInputPhong
{
	float4 pos : POSITION;
	float3 wPos : TEXCOORD0;
	float4 norm : TEXCOORD1; 
};

struct VertexShaderInputTexture
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 texCoordinate : TEXCOORD0;
};

struct PixelShaderInputTexture
{
	float4 pos : POSITION;
	float3 wPos : TEXCOORD0;
	float4 norm : TEXCOORD1; 
	float2 texCoordinate : TEXCOORD2;
};

struct VertexShaderInputBumpMap
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 texCoordinate : TEXCOORD0;
	float3 Tangent : TEXCOORD1;
};

struct PixelShaderInputBumpMap
{
	float4 pos : POSITION;
	float3 wPos : TEXCOORD0;
	float4 norm : TEXCOORD1; 
	float2 texCoordinate : TEXCOORD2;
    float4 vLightPosT : TEXCOORD3;
	float4 vEyePosT : TEXCOORD4;
};

 SamplerState textureSampler : register( s0 ); 
 SamplerState bumpMapSampler : register( s1 );  
 SamplerState atmosMapSampler : register( s2 );

//******************************************
//**                                      **
//**          Constant Colour             **
//**                                      **
//******************************************

float4 M2D_VertexShader(float3 pos : POSITION) : POSITION
{
     float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
     float4 devPos = mul(float4(pos, 1.0), wvpMat);

     return devPos;
}

float4 ConstCol_PixelShader() : COLOR
{
     //return black as pixel colour
     return float4(140.0, 140.0, 140.0, 1.0);
}

technique ConstCol
{
    pass P0    // Effect has only one pass, called 'P0'
    {
          VertexShader = compile vs_2_0 M2D_VertexShader(); //set VertexShader state to vertex shader function       
          PixelShader = compile ps_2_0 ConstCol_PixelShader(); //set PixelShader state to pixel shader function
    }
}


//******************************************
//**                                      **
//**          Normal Colour               **
//**                                      **
//******************************************

VertexShaderOutput M2D_VertexShaderNormal(VertexShaderInput input)
{
	 VertexShaderOutput output;
     float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
     output.pos = mul(float4(input.pos, 1.0), wvpMat);
     output.norm = mul(float4(input.norm, 1.0), wMat);
     return output;
}

float4 ConstCol_PixelShaderNormal(VertexShaderOutput input) : COLOR
{
	input.norm = normalize(input.norm);
	float4 pCol = {(input.norm.x+1)/2, (input.norm.y+1)/2, (input.norm.z+1)/2, 1.0};
	return pCol;
}

technique NormalColor
{
    pass P0  // Effect has only one pass, called 'P0'
    {
          VertexShader = compile vs_2_0 M2D_VertexShaderNormal();
          PixelShader = compile ps_2_0 ConstCol_PixelShaderNormal();
    }
}


//******************************************
//**                                      **
//**          Gouraud Shading             **
//**                                      **
//******************************************

VertexShaderOutput VertexShaderGouraud(VertexShaderInput input)
{
	VertexShaderOutput output;

	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	output.pos = mul(float4(input.pos, 1.0), wvpMat);
	float4 wPos = mul(float4(input.pos, 1.0), wMat);

	//light variables
	float4 vVectCol = {1.0f, 1.0, 1.0f, 1.0f};

	//calc ambient
	float3 ambRefCoef = {0.1f,0.1f,0.1f};
	float3 incAmbInt = {0.1f,0.1f,0.1f};
	float3 refAmbInt = ambRefCoef * incAmbInt;

	float3 ambient;
	ambient.x = vVectCol.x * refAmbInt.x;
	ambient.y = vVectCol.y * refAmbInt.y;
	ambient.z = vVectCol.z * refAmbInt.z;

	//calc diffuse
	float3 difRefCoef = {0.9f,0.9f,0.9f};
	float3 incDifInt = {1.0f,1.0f,1.0f};
	float3 negIncRay = {vLightPos.x - wPos.x, vLightPos.y - wPos.y, vLightPos.z - wPos.z}; //vector pointing from surface to lightsource
	negIncRay = normalize(negIncRay);
	float3 refDifInt = difRefCoef * incDifInt * dot(input.norm, negIncRay);

	float3 diffuse;
	diffuse.x = vVectCol.x * refDifInt.x;
	diffuse.y = vVectCol.y * refDifInt.y;
	diffuse.z = vVectCol.z * refDifInt.z;

	//calc specular (Phong)
	float3 specRefCoef = {0.8f, 0.8f, 0.8f};
	float3 incSpecInt = {0.2f, 0.2f, 0.2f};
	float refShinyCoef = 2.0f;
	float3 scaleConst = shininess * dot(input.norm, negIncRay);
	float3 refRay = scaleConst * input.norm - negIncRay;
	refRay = normalize(refRay);
	float3 negSightVect = { vEyePos.x - wPos.x, vEyePos.y - wPos.y, vEyePos.z - wPos.z };
	negSightVect = normalize(negSightVect);
	float3 refSpecInt = specRefCoef * incSpecInt * (pow(dot(negSightVect, refRay), shininess));

	float3 specular;
	specular.x = vVectCol.x * refSpecInt.x;
	specular.y = vVectCol.y * refSpecInt.y;
	specular.z = vVectCol.z * refSpecInt.z;

	output.norm = float4(ambient + diffuse + specular, transparency);

	return output;
}

float4 PixelShaderGouraud(VertexShaderOutput input) : COLOR
{
	return input.norm;
}

technique ShadingGouraud
{
	pass Pass0
    {
		CullMode = NONE;
		FillMode = SOLID;
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VertexShaderGouraud();
		PixelShader = compile ps_2_0 PixelShaderGouraud();
    }
}


//******************************************
//**                                      **
//**          Phong Shading               **
//**                                      **
//******************************************

PixelShaderInputPhong VertexShaderPhong(VertexShaderInput input)
{
	PixelShaderInputPhong output;

	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	output.pos = mul(float4(input.pos, 1.0), wvpMat);
	output.norm = float4(input.norm, 1.0);
	float4 wPos4 = mul(float4(input.pos, 1.0), wMat);
	output.wPos = float3(wPos4.x, wPos4.y, wPos4.z);
	return output;
}

float4 PixelShaderPhong(PixelShaderInputPhong input) : COLOR
{
	//light variables
	float4 vVectCol = {1.0f, 1.0, 1.0f, 1.0f};

	//calc ambient
	float3 ambRefCoef = {0.1f,0.1f,0.1f};
	float3 incAmbInt = {0.1f,0.1f,0.1f};
	float3 refAmbInt = ambRefCoef * incAmbInt;

	float3 ambient;
	ambient.x = vVectCol.x * refAmbInt.x;
	ambient.y = vVectCol.y * refAmbInt.y;
	ambient.z = vVectCol.z * refAmbInt.z;

	//calc diffuse
	float3 difRefCoef = {0.6f,0.6f,0.6f};
	float3 incDifInt = {0.6f,0.6f,0.6f};
	float3 negIncRay = {vLightPos.x - input.wPos.x, vLightPos.y - input.wPos.y, vLightPos.z - input.wPos.z}; //vector pointing from surface to lightsource
	negIncRay = normalize(negIncRay);
	float3 refDifInt = difRefCoef * incDifInt * dot(input.norm, float4(negIncRay, 1.0f));

	float3 diffuse;
	diffuse.x = vVectCol.x * refDifInt.x;
	diffuse.y = vVectCol.y * refDifInt.y;
	diffuse.z = vVectCol.z * refDifInt.z;

	//calc specular (Phong)
	float3 specRefCoef = {0.5f, 0.5f, 0.5f};
	float3 incSpecInt = {0.5f, 0.5f, 0.5f};
	float refShinyCoef = 0.05f;
	float4 scaleConst = shininess * dot(input.norm, float4(negIncRay,1.0f));
	float4 refRay = scaleConst * (input.norm - float4(negIncRay, 1.0f));
	refRay = normalize(refRay);
	float3 refRay3 = {refRay.x, refRay.y, refRay.z};
	float3 negSightVect = { vEyePos.x - input.wPos.x, vEyePos.y - input.wPos.y, vEyePos.z - input.wPos.z };
	negSightVect = normalize(negSightVect);
	float3 refSpecInt = specRefCoef * incSpecInt * (pow(dot(negSightVect, refRay3), shininess));

	float3 specular;
	specular.x = vVectCol.x * refSpecInt.x;
	specular.y = vVectCol.y * refSpecInt.y;
	specular.z = vVectCol.z * refSpecInt.z;
	
	float4 col = float4(ambient + diffuse + specular, transparency);
	return col;
}

// Technique to implement a 3D transformation pipeline, with culling, z-buffering, etc.
technique ShadingPhong
{
	pass Pass0
    {
		CullMode = NONE;
		FillMode = SOLID;
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VertexShaderPhong();
		PixelShader = compile ps_2_0 PixelShaderPhong();
    }
}

//******************************************
//**                                      **
//**          Texture Mapping             **
//**                                      **
//******************************************

PixelShaderInputTexture VertexShaderTexture(VertexShaderInputTexture input)
{
	PixelShaderInputTexture output;

	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	output.pos = mul(float4(input.pos, 1.0), wvpMat);
	output.norm = float4(input.norm, 1.0);
	output.norm = mul(float4(input.norm, 1.0), wMat);
	output.norm = normalize(output.norm);
	float4 wPos4 = mul(float4(input.pos, 1.0), wMat);
	output.wPos = float3(wPos4.x, wPos4.y, wPos4.z);
	output.texCoordinate = input.texCoordinate;
	return output;
}

float4 PixelShaderTexture(PixelShaderInputTexture input) : COLOR
{
	//light variables
	float4 vVectCol = {0.7f, 0.7f, 0.7f, 1.0f};

	//calc ambient
	float3 ambRefCoef = {1.0f,1.0f,1.0f};
	float3 incAmbInt = {1.0f,1.0f,1.0f};
	float3 refAmbInt = ambRefCoef * incAmbInt;

	float3 ambient;
	ambient.x = vVectCol.x * refAmbInt.x;
	ambient.y = vVectCol.y * refAmbInt.y;
	ambient.z = vVectCol.z * refAmbInt.z;

	//calc diffuse
	float3 difRefCoef = {1.0f,1.0f,1.0f};
	float3 incDifInt = {1.0f,1.0f,1.0f};
	float3 negIncRay = {vLightPos.x - input.wPos.x, vLightPos.y - input.wPos.y, vLightPos.z - input.wPos.z}; //vector pointing from surface to lightsource
	negIncRay = normalize(negIncRay);
	float3 refDifInt = difRefCoef * incDifInt * dot(input.norm, float4(negIncRay, 1.0f));

	float3 diffuse;
	diffuse.x = vVectCol.x * refDifInt.x;
	diffuse.y = vVectCol.y * refDifInt.y;
	diffuse.z = vVectCol.z * refDifInt.z;

	//calc specular (Phong)
	float3 specRefCoef = {0.1f, 0.1f, 0.1f};
	float3 incSpecInt = {1.0f,1.0f,1.0f};
	float refShinyCoef = 0.000001f;
	float4 scaleConst = shininess * dot(input.norm, float4(negIncRay,1.0f));
	float4 refRay = scaleConst * (input.norm - float4(negIncRay, 1.0f));
	refRay = normalize(refRay);
	float3 refRay3 = {refRay.x, refRay.y, refRay.z};
	float3 negSightVect = { vEyePos.x - input.wPos.x, vEyePos.y - input.wPos.y, vEyePos.z - input.wPos.z };
	negSightVect = normalize(negSightVect);
	float3 refSpecInt = specRefCoef * incSpecInt * (pow(dot(negSightVect, refRay3), shininess));

	float3 specular;
	specular.x = vVectCol.x * refSpecInt.x;
	specular.y = vVectCol.y * refSpecInt.y;
	specular.z = vVectCol.z * refSpecInt.z;
	
	float4 textureColor = tex2D(textureSampler, input.texCoordinate);

	float4 col = float4(textureColor * (ambient + diffuse + specular), textureColor.a);
	return col;
}

PixelShaderInputTexture VS_OuterAtmoshpere(VertexShaderInputTexture input,uniform float size)
{
	PixelShaderInputTexture output = (PixelShaderInputTexture)0;
	output.norm = mul(input.norm, wMat);
	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	output.pos = mul(input.pos, wvpMat) + (mul(size, mul(input.norm, wvpMat)));
	output.texCoordinate = input.texCoordinate;
	output.wPos = mul(input.pos,wMat);
	
	return output;
}

float4 PS_OuterAtmoshpere(PixelShaderInputTexture input,uniform bool flip) : COLOR
{
	float4 output = (float4)0;
	
	// Do light scatter...
	float4 atmos = tex2D(atmosMapSampler, input.texCoordinate);
	

	float3 regN = normalize(input.norm);
	float3 Half = normalize(normalize(vEyePos - input.pos) + normalize(vEyePos - input.pos));	
	float specular = 0;
		
	if(flip)
	{		
		specular = (saturate(1-dot(regN,Half))*.125);
		atmos.a *= specular;
	}
	else
	{
		//specular = 1-saturate(1.125 + dot(regN,Half));
		specular = 1-saturate(1.1 + dot(regN,Half));
		//atmos *= specular;
	}
	output = atmos;// * Diffuse;
	
	return output;
}

technique ShadingTexture
{
	pass Pass0
    {
		CullMode = NONE;
		FillMode = SOLID;
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VertexShaderTexture();
		PixelShader = compile ps_2_0 PixelShaderTexture();
    }
    
    /*pass Pass1
	{        
		VertexShader = compile vs_2_0 VS_OuterAtmoshpere(.2);
		PixelShader = compile ps_2_0 PS_OuterAtmoshpere(false);
		CullMode = CW;
	}*/
}

//******************************************
//**                                      **
//**          Texture Mapping             **
//**                                      **
//******************************************

PixelShaderInputTexture VertexShaderBasicTexture(VertexShaderInputTexture input)
{
	PixelShaderInputTexture output;

	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	output.pos = mul(float4(input.pos, 1.0), wvpMat);
	output.norm = float4(input.norm, 1.0);
	float4 wPos4 = mul(float4(input.pos, 1.0), wMat);
	output.wPos = float3(wPos4.x, wPos4.y, wPos4.z);
	output.texCoordinate = input.texCoordinate;
	return output;
}

float4 PixelShaderBasicTexture(PixelShaderInputTexture input) : COLOR
{
	//light variables
	float4 vVectCol = {0.6f, 0.6, 0.6f, 1.0f};

	//calc ambient
	float3 ambRefCoef = {0.1f,0.1f,0.1f};
	float3 incAmbInt = {1.0f,1.0f,1.0f};
	float3 refAmbInt = ambRefCoef * incAmbInt;

	float3 ambient;
	ambient.x = vVectCol.x * refAmbInt.x;
	ambient.y = vVectCol.y * refAmbInt.y;
	ambient.z = vVectCol.z * refAmbInt.z;

	//calc diffuse
	float3 difRefCoef = {1.0f,1.0f,1.0f};
	float3 incDifInt = {1.0f,1.0f,1.0f};
	float3 negIncRay = {vLightPos.x - input.wPos.x, vLightPos.y - input.wPos.y, vLightPos.z - input.wPos.z}; //vector pointing from surface to lightsource
	negIncRay = normalize(negIncRay);
	float3 refDifInt = difRefCoef * incDifInt * dot(input.norm, float4(negIncRay, 1.0f));

	float3 diffuse;
	diffuse.x = vVectCol.x * refDifInt.x;
	diffuse.y = vVectCol.y * refDifInt.y;
	diffuse.z = vVectCol.z * refDifInt.z;

	//calc specular (Phong)
	float3 specRefCoef = {0.1f, 0.1f, 0.1f};
	float3 incSpecInt = {1.0f,1.0f,1.0f};
	float refShinyCoef = 0.000001f;
	float4 scaleConst = shininess * dot(input.norm, float4(negIncRay,1.0f));
	float4 refRay = scaleConst * (input.norm - float4(negIncRay, 1.0f));
	refRay = normalize(refRay);
	float3 refRay3 = {refRay.x, refRay.y, refRay.z};
	float3 negSightVect = { vEyePos.x - input.wPos.x, vEyePos.y - input.wPos.y, vEyePos.z - input.wPos.z };
	negSightVect = normalize(negSightVect);
	float3 refSpecInt = specRefCoef * incSpecInt * (pow(dot(negSightVect, refRay3), shininess));

	float3 specular;
	specular.x = vVectCol.x * refSpecInt.x;
	specular.y = vVectCol.y * refSpecInt.y;
	specular.z = vVectCol.z * refSpecInt.z;
	
	float4 textureColor = tex2D(textureSampler, input.texCoordinate);

	return textureColor;
}

technique BasicTexture
{
	pass Pass0
    {
		CullMode = NONE;
		FillMode = SOLID;
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VertexShaderBasicTexture();
		PixelShader = compile ps_2_0 PixelShaderBasicTexture();
    }
}

//******************************************
//**                                      **
//**             Bump Mapping             **
//**                                      **
//******************************************

PixelShaderInputBumpMap VertexShaderBumpMap(VertexShaderInputBumpMap input)
{
	PixelShaderInputBumpMap output;

	float4x4 wvpMat = mul(mul(wMat, vMat), pMat);
	float3 row1 = mul(float4(input.Tangent, 1.0), wMat);
	float3 row3 = mul(float4(input.norm, 1.0), wMat);
	float3 row2 = mul(float4(cross(input.Tangent, input.norm),1.0), wMat);
	row1 = normalize(row1);
	row2 = normalize(row2);
	row3 = normalize(row3);
	float3x3 vertTangentMatrix = {row1.x, row1.y, row1.z,
								  row2.x, row2.y, row2.z,
								  row3.x, row3.y, row3.z};
	output.pos = mul(float4(input.pos, 1.0), wvpMat);
	output.norm = float4(input.norm, 1.0);
	float4 wPos4 = mul(float4(input.pos, 1.0), wMat);
	output.wPos = float3(wPos4.x, wPos4.y, wPos4.z);
	output.texCoordinate = input.texCoordinate;
	output.vLightPosT = float4(mul(vertTangentMatrix, vLightPos - wPos4),1.0f);
	output.vEyePosT =  float4(mul(vertTangentMatrix, vEyePos - wPos4),1.0f);
	return output;
}

float4 PixelShaderBumpMap(PixelShaderInputBumpMap input) : COLOR
{
	//light variables
	float4 vVectCol = {0.6f, 0.6, 0.6f, 1.0f};
	
	float4 textureColor = tex2D(textureSampler, input.texCoordinate);
	textureColor.a = 1;
	float4 normColor = tex2D(bumpMapSampler, input.texCoordinate);
	normColor = normColor * 2;
	normColor = normColor - 1.0f;
	normColor.a = 1;

	//calc ambient
	float3 ambRefCoef = {0.1f,0.1f,0.1f};
	float3 incAmbInt = {1.0f,1.0f,1.0f};
	float3 refAmbInt = ambRefCoef * incAmbInt;

	float3 ambient;
	ambient.x = vVectCol.x * refAmbInt.x;
	ambient.y = vVectCol.y * refAmbInt.y;
	ambient.z = vVectCol.z * refAmbInt.z;

	//calc diffuse
	float3 difRefCoef = {1.0f,1.0f,1.0f};
	float3 incDifInt = {1.0f,1.0f,1.0f};
	float3 negIncRay = input.vLightPosT; //vector pointing from surface to lightsource
	negIncRay = normalize(negIncRay);
	float3 refDifInt = difRefCoef * incDifInt * dot(normColor, float4(negIncRay, 1.0f));


	float3 diffuse;
	diffuse.x = vVectCol.x * refDifInt.x;
	diffuse.y = vVectCol.y * refDifInt.y;
	diffuse.z = vVectCol.z * refDifInt.z;

	//calc specular (Phong)
	float3 specRefCoef = {0.1f, 0.1f, 0.1f};
	float3 incSpecInt = {1.0f,1.0f,1.0f};
	float refShinyCoef = 0.000001f;
	float4 scaleConst = 2.0 * dot(normColor, float4(negIncRay,1.0f));
	float4 refRay = (scaleConst * normColor) - float4(negIncRay, 1.0f);
	refRay = normalize(refRay);
	float3 refRay3 = {refRay.x, refRay.y, refRay.z};
	float3 negSightVect = {input.vEyePosT.x, input.vEyePosT.y, input.vEyePosT.z};
	negSightVect = normalize(negSightVect);
	float3 refSpecInt = specRefCoef * incSpecInt * (pow(dot(negSightVect, refRay3), shininess));

	float3 specular;
	specular.x = vVectCol.x * refSpecInt.x;
	specular.y = vVectCol.y * refSpecInt.y;
	specular.z = vVectCol.z * refSpecInt.z;
	
	float4 col;

	if (input.vLightPosT.z <= 0)
	{ 
		refDifInt = 0;
		refSpecInt = 0;
		specular.x = vVectCol.x * refSpecInt.x;
		specular.y = vVectCol.y * refSpecInt.y;
		specular.z = vVectCol.z * refSpecInt.z;
		diffuse.x = vVectCol.x * refDifInt.x;
		diffuse.y = vVectCol.y * refDifInt.y;
		diffuse.z = vVectCol.z * refDifInt.z;
	}
	else
	{
		// apply illumination model(s) to shade pixel
		col = (float4((textureColor * (ambient + diffuse))+ specular, textureColor.a));
	}
	return col;
}

technique ShadingBumpMap
{
	pass Pass0
    {
		CullMode = NONE;
		FillMode = SOLID;
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_2_0 VertexShaderBumpMap();
		PixelShader = compile ps_2_0 PixelShaderBumpMap();
    }
}