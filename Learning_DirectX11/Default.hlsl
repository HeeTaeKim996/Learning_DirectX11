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
    float4 offset;
}



// LA - VS -RS - PS - OM �� VS(vertex Shader) �� RS �� ���� �ڵ�    ( �켱�� ���� ������ ��� )
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position + offset;
    //output.color = input.color;
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
    float2 uv : TEXXCORD;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
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