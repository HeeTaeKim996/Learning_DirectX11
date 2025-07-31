#if 0 // 주석

/* ■ hlsl 파일 선택 -> 우클릭 -> 속성 정리

    ○ 진입점 이름    : 기본 main. 현재 작성 기준 main 에서, VS (아래함수) 로 수정

    ○ 셰이더 모델    : Shader Model 5.0(/5_0) 로 수정해서 사용함 (차이점은 모름)
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

// LA - VS -RS - PS - OM 중 VS(vertex Shader) 와 RS 에 대한 코드    ( 우선은 아주 간단한 양식 )
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



#endif // 주석


















































































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