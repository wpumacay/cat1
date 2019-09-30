#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexcoord;

out vec4 fColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;
    int diffuseMapActive;
    sampler2D specularMap;
    int specularMapActive;
    sampler2D normalMap;
    int normalMapActive;
};
uniform Material u_material;

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    int enabled;

    vec3 position;
};
uniform PointLight u_pointLight;

struct DirectionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    int enabled;

    vec3 direction;
};
uniform DirectionalLight u_directionalLight;

uniform vec3 u_viewerPosition;

vec3 _computeLightAmbientFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return u_directionalLight.intensity * u_directionalLight.ambient;

    if ( u_pointLight.enabled == 1 )
        return u_directionalLight.intensity * u_pointLight.ambient;

    // default value, in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
}

vec3 _computeLightDiffuseFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return max( dot( normalize( -u_directionalLight.direction ),
                         normalize( fNormal ) ), 0.0f ) * u_directionalLight.intensity * u_directionalLight.diffuse;

    if ( u_pointLight.enabled == 1 )
        return max( dot( normalize( u_pointLight.position - fPosition ),
                         normalize( fNormal ) ), 0.0f ) * u_pointLight.intensity * u_pointLight.diffuse;

    // default value (constant everywhere) in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
}

vec3 _computeLightSpecularFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return pow( max( dot( normalize( reflect( normalize( u_directionalLight.direction ), fNormal ) ),
                              normalize( u_viewerPosition - fPosition ) ), 0.0f ), u_material.shininess ) * u_directionalLight.intensity * u_directionalLight.specular;

    if ( u_pointLight.enabled == 1 )
        return pow( max( dot( normalize( reflect( -normalize( u_pointLight.position - fPosition ), fNormal ) ),
                              normalize( u_viewerPosition - fPosition ) ), 0.0f ), u_material.shininess ) * u_pointLight.intensity * u_pointLight.specular;

    // default value (constant everywhere) in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
}

vec3 _computeObjectAmbientComp( vec3 lightAmbientComp )
{
    if ( u_material.diffuseMapActive == 1 )
        return vec3( texture( u_material.diffuseMap, fTexcoord ) ) * lightAmbientComp;

    return u_material.ambient * lightAmbientComp;
}

vec3 _computeObjectDiffuseComp( vec3 lightDiffuseComp )
{
    if ( u_material.diffuseMapActive == 1 )
        return vec3( texture( u_material.diffuseMap, fTexcoord ) ) * lightDiffuseComp;

    return u_material.diffuse * lightDiffuseComp;
}

vec3 _computeObjectSpecularComp( vec3 lightSpecularComp )
{
    if ( u_material.specularMapActive == 1 )
        return vec3( texture( u_material.specularMap, fTexcoord ) ) * lightSpecularComp;

    return u_material.specular * lightSpecularComp;
}

void main()
{
    vec3 _lAmbientFactor = _computeLightAmbientFactor();
    vec3 _lDiffuseFactor = _computeLightDiffuseFactor();
    vec3 _lSpecularFactor = _computeLightSpecularFactor();

    vec3 _oAmbientComp = _computeObjectAmbientComp( _lAmbientFactor );
    vec3 _oDiffuseComp = _computeObjectDiffuseComp( _lDiffuseFactor );
    vec3 _oSpecularComp = _computeObjectSpecularComp( _lSpecularFactor );

    fColor = vec4( _oAmbientComp + _oDiffuseComp + _oSpecularComp, 1.0f );
}