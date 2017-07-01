attribute vec3 vertexPosition_modelSpace;
attribute vec2 UV;
attribute vec3 vertexNormal_modelSpace;
attribute vec3 vertexTangent_modelSpace;
attribute vec3 vertexBinormal_modelSpace;
//attribute vec3 normal;
//#version 330 core
//layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec2 UV;

uniform mat4 uniMVP;
uniform mat4 uniViewMatrix;
uniform mat4 uniModelMatrix;
uniform vec3 uniLightPosition_worldSpace;
uniform mat3 uniModelView3x3;

//out vec2 vertexUV;
varying vec2 varUV;
varying vec3 varNormal_cameraSpace;
varying vec3 varPosition_worldSpace;
varying vec3 varEyeDirection_cameraSpace;
varying vec3 varLightDirection_cameraSpace;
varying vec3 varLightPosition_worldSpace;
//varying vec3 fragmentColor;
varying vec3 varLightDirection_tangentSpace;
varying vec3 varEyeDirection_tangentSpace;

varying vec3 tangent;
varying vec3 bitangent;

void main() {
    //fragmentColor = color;
    gl_Position = uniMVP * vec4(vertexPosition_modelSpace, 1.0);
    
    varPosition_worldSpace = (uniModelMatrix * vec4(vertexPosition_modelSpace, 1.0)).xyz;
    varLightPosition_worldSpace = uniLightPosition_worldSpace;
    
    vec3 vertexPosition_cameraSpace = (uniViewMatrix *
                                       uniModelMatrix *
                                       vec4(vertexPosition_modelSpace, 1.0)).xyz;
    
    vec3 eyeDirection_cameraSpace = vec3(0.0, 0.0, 0.0) - vertexPosition_cameraSpace;
    
    varEyeDirection_cameraSpace = eyeDirection_cameraSpace;
    
    vec3 lightPosition_cameraSpace = (uniViewMatrix * vec4(uniLightPosition_worldSpace, 1.0)).xyz;
    vec3 lightDirection_cameraSpace = lightPosition_cameraSpace + eyeDirection_cameraSpace;
    
    varLightDirection_cameraSpace = lightDirection_cameraSpace;
    varNormal_cameraSpace = (uniViewMatrix * uniModelMatrix * vec4(vertexNormal_modelSpace, 0.0)).xyz;
    
    varUV = UV;
    
    vec3 vertexTangent_cameraSpace = normalize(uniModelView3x3 * vertexTangent_modelSpace);
    vec3 vertexNormal_cameraSpace = normalize(uniModelView3x3 * vertexNormal_modelSpace);
    vec3 vertexBinormal_cameraSpace = normalize(uniModelView3x3 * cross(vertexNormal_modelSpace, vertexTangent_modelSpace));
    
    mat3 TBN = mat3(
        vertexTangent_cameraSpace.x, vertexBinormal_cameraSpace.x, vertexNormal_cameraSpace.x,
        vertexTangent_cameraSpace.y, vertexBinormal_cameraSpace.y, vertexNormal_cameraSpace.y,
        vertexTangent_cameraSpace.z, vertexBinormal_cameraSpace.z, vertexNormal_cameraSpace.z
    );
    
    varLightDirection_tangentSpace = TBN * lightDirection_cameraSpace;
    varEyeDirection_tangentSpace = TBN * eyeDirection_cameraSpace;
    
    tangent = normalize(vertexTangent_modelSpace);
    bitangent = normalize(cross(vertexNormal_modelSpace, vertexTangent_modelSpace)); //???
    
    //varNormal_cameraSpace = (uniViewMatrix * uniModelMatrix * vec4(vertexNormal_modelSpace, 1.0)).xyz;
}
