#version 330 core

out vec4 FragColor;

// from Vertex shader
in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_posLightSpace;

// Material
struct Material {
    vec3 ambient;

    vec3 diffuse;
    sampler2D diffuseMap;

    float shininess;
    vec3 specular;
    sampler2D specularMap;

    float opacity;
};

uniform Material u_material;
uniform sampler2D u_shadowMap;

// light
uniform vec3 ambientLight;
uniform vec3 diffuseLightDir;
uniform vec3 diffuseLightColor;
uniform vec3 u_viewPos;

// shadow calculations
float shadowCalculation(vec4 fragPosLightSpace) {
    // return 0.0;

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;

    // check whether current frag pos is in shadow
    // float shadow = projCoords.z > closestDepth  ? 1.0 : 0.0;

// /*
    float shadow = 0.0f;
    // blur
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - 0.0002f > pcfDepth ? 1.0 : 0.0;
        };
    };
    shadow /= 9;
// */
    return shadow;
};

// for fog
uniform float u_near;
uniform float u_far;
uniform vec3 u_fogColor;
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near)) / u_far;
};

void main() {
    // shadow calculations
    float shadow = shadowCalculation(v_posLightSpace);

    // calculate ambient light
    vec3 ambient = ambientLight * u_material.ambient;

    // calculate defuse light
    float diffuse_diff = max(dot(v_normal, diffuseLightDir), 0.0f);
    vec3 diffuse = diffuse_diff * diffuseLightColor * u_material.diffuse;

    // calculate specular light
    vec3 viewDir = normalize(u_viewPos - v_pos);
    vec3 reflectDir = reflect(diffuseLightDir, v_normal);
    float spec = pow(max(dot(viewDir, -reflectDir), 0.0f), u_material.shininess);
    vec3 specular = spec * diffuseLightColor * u_material.specular;

    // set textures
    vec4 diff_texture = texture(u_material.diffuseMap, v_uv);
    ambient *= vec3(diff_texture);
    diffuse *= vec3(diff_texture);
    specular *= vec3(texture(u_material.specularMap, v_uv));

    // final color result
    FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), u_material.opacity * diff_texture.a);

    // mix with  fog
    FragColor = mix(FragColor, vec4(u_fogColor, 1.0f), LinearizeDepth(gl_FragCoord.z));
/*
    //--------------------------------------------------------------------------
    // grayScale
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
*/
};
