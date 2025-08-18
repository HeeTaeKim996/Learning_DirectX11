#if 1 // �ּ�

/* �� hlsl ���� ���� -> ��Ŭ�� -> �Ӽ� ����

    �� ������ �̸�    : �⺻ main. ���� �ۼ� ���� main ����, VS (�Ʒ��Լ�) �� ����

    �� ���̴� ��    : Shader Model 5.0(/5_0) �� �����ؼ� ����� (�������� ��)
*/

struct VS_INPUT
{
    float4 position : POSITION;
    // float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    // float4 color : COLOR;
    float2 uv : TEXCOORD;
};



cbuffer TransformData : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
    
    /*  �� row_major
         - �̵���� ���Ӽ����� P V (TRS) v �� �߾��µ�,
           ���� �������� ������ v (SRT) V P �� ����ϰ� �ִ�. (���� ���� ����� ��ġ�� ����)
           ���ڴ� Vec���� Mat �����, Vec�� ColVec �̾��µ�, ���ڴ� ��ġ�Ǵ�, Vec�� RowVec���� �ν��ؾ���. 
           �� row_major�� �� ����. Matrix ���� �Է�, ���Ǵ� Vec ���� rowVec ���� �����϶�� ��
    */
    
}



// LA - VS -RS - PS - OM �� VS(vertex Shader) �� RS �� ���� �ڵ�    ( �켱�� ���� ������ ��� )
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // WVP
    float4 position = mul(input.position, matWorld); // W
    position = mul(position, matView); // V
    position = mul(position, matProjection); // P
    
    
    output.position = position;
    output.uv = input.uv;
    

    return output;
}


Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
/*  
    �� Texture2D     : hlsl�� ���� Ÿ������, �ؽ���
    �� SamplerState  : hlsl�� ���� Ÿ������, �ؽ��ĸ� ���ø��ϴ� ���
    
    �� register(n)   : ����� ���� ���� -> n �� �ε��� ���� ����

    -> texture0 �� Texture ���ҽ� ������ 0�� �� �ǹ�
       SamplerState �� SamplerState ������ 0�� �� �ǹ�
*/

float4 PS(VS_OUTPUT input) : SV_Target
{   
    //return input.color;  
    
    float4 color = texture0.Sample(sampler0, input.uv);
    /*  �� texture0.Sample(sampler0, input.uv)
            -> ���ڷ� ���� input.uv ��ǥ�� ��������, 
               texture0�� 0�� �ؽ��� ������ �� �� �����Ͽ�, 
        `      sampler0 �� 0�� ���÷� ���� �� �������,
               ���ø��Ͽ� �÷��� ��ȯ
    */

    
    return color;
}



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}   



#endif // �ּ�


















































































#if 0  // Practice

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    float4 offset;
}


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position + offset;
    output.uv = input.uv;
    
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.uv);
    
    return color;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}

#endif // Practice