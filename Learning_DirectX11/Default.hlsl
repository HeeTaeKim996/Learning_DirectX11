#if 1 // 주석

/* ■ hlsl 파일 선택 -> 우클릭 -> 속성 정리

    ○ 진입점 이름    : 기본 main. 현재 작성 기준 main 에서, VS (아래함수) 로 수정

    ○ 셰이더 모델    : Shader Model 5.0(/5_0) 로 수정해서 사용함 (차이점은 모름)
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
    
    /*  □ row_major
         - 이득우저 게임수학은 P V (TRS) v 로 했었는데,
           현재 공부중인 엔진은 v (SRT) V P 를 사용하고 있다. (기존 수리 행렬의 전치와 같음)
           전자는 Vec으로 Mat 연산시, Vec이 ColVec 이었는데, 후자는 전치되니, Vec이 RowVec으로 인식해야함. 
           위 row_major는 그 내용. Matrix 에서 입력, 사용되는 Vec 들을 rowVec 으로 간주하라는 뜻
    */
    
}



// LA - VS -RS - PS - OM 중 VS(vertex Shader) 와 RS 에 대한 코드    ( 우선은 아주 간단한 양식 )
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
    ○ Texture2D     : hlsl의 내장 타입으로, 텍스쳐
    ○ SamplerState  : hlsl의 내장 타입으로, 텍스쳐를 샘플링하는 방법
    
    ○ register(n)   : 명시적 슬롯 지정 -> n 번 인덱스 슬롯 매핑

    -> texture0 은 Texture 리소스 슬롯의 0번 을 의미
       SamplerState 은 SamplerState 슬롯의 0번 을 의미
*/

float4 PS(VS_OUTPUT input) : SV_Target
{   
    //return input.color;  
    
    float4 color = texture0.Sample(sampler0, input.uv);
    /*  ○ texture0.Sample(sampler0, input.uv)
            -> 인자로 들어온 input.uv 좌표를 기준으로, 
               texture0인 0번 텍스쳐 슬롯의 값 을 참조하여, 
        `      sampler0 인 0번 샘플러 슬롯 의 방법으로,
               샘플링하여 컬러를 반환
    */

    
    return color;
}



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}   



#endif // 주석


















































































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