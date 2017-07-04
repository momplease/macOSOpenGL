//#version 330
//in vec2 vertexUV;
varying vec2 varUV;
varying vec3 varPosition_worldSpace;
varying vec3 varNormal_cameraSpace;
varying vec3 varEyeDirection_cameraSpace;
varying vec3 varLightDirection_cameraSpace;

varying vec3 varLightPosition_worldSpace;
//varying vec3 fragmentColor;
varying vec3 varLightDirection_tangentSpace;
varying vec3 varEyeDirection_tangentSpace;

varying vec3 tangent;

//out vec4 fragColor;
//uniform float uniPowerOfLight;
uniform sampler2D uniDiffuseTexture;
uniform sampler2D uniNormalMap;
uniform mat4 uniModelView;


void main(){
    //gl_FragColor = vec4(fragmentColor, 1.0);
    //vec2 v = vertexUV;
    //fragColor = vec4(texture(textureSampler, vec2(vertexUV.x, 1.0 - vertexUV.y)).rgb, 1.0);
    //vec4 dummy = texture2D(diffuseTexture1, vertexUV);1

    // Specially for FBX !!! Workaround type of code
    //if (vertexUV2.x > 1.0 || vertexUV2.x < 0.0 || vertexUV2.y > 1.0 || vertexUV2.y < 0.0)
    //    discard;
    
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float lightPower = 50.0;
    
    //vec3 materialNormalColor = texture2D(uniNormalMap, vec2(varUV.x, 1.0 - varUV.y)).xyz;
    vec4 diff = (texture2D(uniDiffuseTexture, vec2(varUV.x, 1.0 - varUV.y)));
    
    vec3 materialDiffuseColor = (texture2D(uniDiffuseTexture, vec2(varUV.x, 1.0 - varUV.y))).xyz;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3, 0.3, 0.3);
    
    float dist = length(varLightPosition_worldSpace - varPosition_worldSpace);
    
    //temp
    vec3 vvv = varNormal_cameraSpace;
    vec3 vvvv = varEyeDirection_cameraSpace;
    vec3 vvvvv = varLightDirection_cameraSpace;
    
    vec3 textureNormal_tangentSpace = normalize(texture2D(uniNormalMap, vec2(varUV.x, 1.0 - varUV.y)).rgb * 2.0 - 1.0);
    
    //vec3 n = varNormal_cameraSpace;
    vec3 n = textureNormal_tangentSpace;
    vec3 lightDirection = normalize(varLightDirection_tangentSpace);
    //lightDirection = normalize(varLightDirection_cameraSpace);
    
    float cosTheta = clamp(dot(n, lightDirection), 0.0, 1.0);
    
    vec3 eyeVector = normalize(varEyeDirection_tangentSpace);
    //eyeVector = normalize(varEyeDirection_cameraSpace);
    
    vec3 reflectionVector = reflect(-lightDirection, n);
    
    float cosAlpha = clamp(dot(eyeVector, reflectionVector), 0.0, 1.0);

    //gl_FragColor = vec4(materialDiffuseColor * lightPower * lightColor * cosTheta / (dist / 35.0), 1.0);
    gl_FragColor = vec4(materialAmbientColor +
                        materialDiffuseColor * lightColor * lightPower * cosTheta / (dist) +
                        materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 32.0) / (dist), diff.a);
    
    vec3 t = tangent;
    //gl_FragColor = vec4(tangent, 1.0);
    //vec4(vec2(varUV.x, 1.0 - varUV.y), 0.0, 1.0);
    //gl_FragColor = vec4(diff.xyz, min( diff.a+(1.0-step( 1.0, diff.a )), 1.0));
    //gl_FragColor = vec4(textureNormal_tangentSpace, 1.0);
    //vec4(lightPower * lightColor.r * (cosTheta / (dist)), 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(materialAmbientColor
    //             + materialDiffuseColor * lightColor * lightPower * cosTheta / (dist * dist)
    //             + materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 10.0) / (dist * dist), 1.0);
    
    
    //vec4 diffTex1Color = texture2D(diffuseTexture1, vec2(vertexUV2.x, 1.0 - vertexUV2.y));
    
    //gl_FragColor = diffuseColor * uniColorOfLight * uniPowerOfLight * cosTheta / (uniDistance * uniDistance);
    //gl_FragColor = diffTex1Color;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
