#version 330 core

out vec4 FragColor;

in vec2 v_uv;

uniform sampler2D screenTexture;

// /*
void main() {
    FragColor = texture(screenTexture, v_uv);
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(vec3(average), 1.0);
};
// */
/*
const float offset = 1.0 / 250.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, v_uv.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
};
*/
/*
// uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
    vec3 result = texture(screenTexture, v_uv).rgb * weight[0]; // current fragment's contribution
    if(true) {
        for(int i = 1; i < 5; ++i) {
            result += texture(screenTexture, v_uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, v_uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, v_uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(screenTexture, v_uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
*/
