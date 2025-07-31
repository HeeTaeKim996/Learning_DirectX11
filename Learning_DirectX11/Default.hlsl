#if 0 // �ּ�

/* �� hlsl ���� ���� -> ��Ŭ�� -> �Ӽ� ����

    �� ������ �̸�    : �⺻ main. ���� �ۼ� ���� main ����, VS (�Ʒ��Լ�) �� ����

    �� ���̴� ��    : Shader Model 5.0(/5_0) �� �����ؼ� ����� (�������� ��)
*/

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// LA - VS -RS - PS - OM �� VS(vertex Shader) �� RS �� ���� �ڵ�    ( �켱�� ���� ������ ��� )
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
    output.color = input.color;
    
    

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;  
}



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}   



#endif // �ּ�


















































































#if 1  // Practice
struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
    output.color = input.color;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}








#endif // Practice