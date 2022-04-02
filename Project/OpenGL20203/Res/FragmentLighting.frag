#version 450

// 入力変数
layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexcoord;
layout(location=2) in vec3 inNormal;
//出力変数
out vec4 fragColor;

//ユニフォーム変数
layout(binding=0) uniform sampler2D texColor;

// 平行高原
struct DirectionalLight
{
	vec3 direction;	// ライトの向き
	vec3 color;		// ライトの色
};
DirectionalLight light = 
{
	{ 0.65,-0.63,-0.43},
	{ 1.81, 1.16, 0.32},
};

// 環境光の色(明るさ)
vec3 ambientLight = { 0.15, 0.10, 0.20 };

void main() {
  vec4 tc = texture(texColor, inTexcoord);
  fragColor = inColor * tc;

  // 線形補間によって長さが1ではなくなっているので、正規化して長さを1に復元する
  vec3 worldNormal = normalize(inNormal);

  // 面が裏向きの場合、法線の向きを逆にする
  if (gl_FrontFacing == false)
  {
	worldNormal = -worldNormal;
  }

  // 環境光の設定
  vec3 lightColor = ambientLight;

  // ランパート反社による明るさを計算
  float cosTheta = max(dot(worldNormal, -light.direction), 0);
  lightColor += light.color * cosTheta;

  fragColor.rgb *= lightColor;
}