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

// textures
struct Textures {
    sampler2D grass;
    sampler2D grass_specular;

    sampler2D rock;
    sampler2D rock_specular;
};
uniform Textures u_textures;
uniform float u_gradientFactor;

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

    // blur
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z > pcfDepth ? 1.0 : 0.0;  // projCoords.z - 0.0002f // for bias
        };
    };
    shadow /= 9;

    // int blurDefinition = 20;
    // for(int x = -blurDefinition/2; x <= blurDefinition/2; ++x) {
    //     for(int y = -blurDefinition/2; y <= -blurDefinition/2; ++y) {
    //         float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
    //         shadow += projCoords.z > pcfDepth ? 1.0 : 0.0;
    //     };
    // };
    // shadow /= blurDefinition+1;
// */
    return shadow;
};

// fog
uniform vec3 u_fogColor;
in float v_visibility;

vec3 diffuse_texture, specular_texture;
float gradCoeficient;
void main() {
    // shadow calculations
    float shadow = shadowCalculation(v_posLightSpace);

    //--------------------------------------------------------------------------
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

// /*
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // set textures
    float gradient = clamp(abs(dot(vec3(0.0f, 1.0f, 0.0f), v_normal)) - u_gradientFactor, 0.0f, 1.0f);

    // grass texture
    gradCoeficient = gradient;

    diffuse_texture = vec3(texture(u_textures.grass, v_uv)) * gradCoeficient;
    specular_texture = vec3(texture(u_textures.grass_specular, v_uv)) * gradCoeficient;

    //--------------------------------------------------------------------------
    // rock texture
    gradCoeficient = 1.0f - gradient;

    diffuse_texture += vec3(texture(u_textures.rock, v_uv)) * gradCoeficient;
    specular_texture += vec3(texture(u_textures.rock_specular, v_uv)) * gradCoeficient;

    // multiply by lighting
    ambient *= diffuse_texture;
    diffuse *= diffuse_texture;
    specular *= specular_texture;

    // calculate final fragment color
    FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), u_material.opacity);
// */

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // mix with  fog
    FragColor = mix(vec4(u_fogColor, 1.0f), FragColor, v_visibility);
// */
};
