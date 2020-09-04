const int MAX_JOINTS = 64;
const int JOINTS_PER_VERTEX = 3;

attribute vec3 positionAttribute;
attribute vec3 normalAttribute;
attribute vec2 uvAttribute;
attribute vec3 jointIndicesAttribute;
attribute vec3 jointWeightsAttribute;

uniform mat4 mvpMatrixUniform;
uniform mat4 modelMatrixUniform;
/*uniform mat4 lightMvpMatrixUniform;
uniform mat4 biasMatrixUniform;*/
uniform mat4 jointTransformsUniform[MAX_JOINTS];
uniform bool hasSkeletalAnimationUniform;

varying vec2 uvVarying;
varying vec3 normalVarying;
varying vec3 positionVarying;
//varying vec4 shadowMapUvVariying;

void main() {
    uvVarying = uvAttribute;
    //shadowMapUvVariying = biasMatrixUniform * lightMvpMatrixUniform * vec4(vertexCoordinateAttribute, 1.0);

    if (hasSkeletalAnimationUniform) {
        vec4 finalPosition = vec4(0.0);
        vec4 finalNormal = vec4(0.0);

        for (int i = 0; i < JOINTS_PER_VERTEX; i++) {
            mat4 jointTransform = jointTransformsUniform[int(jointIndicesAttribute[i])];
            vec4 posedVertexPosition = jointTransform * vec4(positionAttribute, 1.0);
            finalPosition += posedVertexPosition * jointWeightsAttribute[i];

            vec4 posedNormal = jointTransform * vec4(normalAttribute, 0.0);
            finalNormal += posedNormal * jointWeightsAttribute[i];
        }

        normalVarying = (modelMatrixUniform * finalNormal).xyz;
        positionVarying = (modelMatrixUniform * finalPosition).xyz;
        gl_Position = mvpMatrixUniform * finalPosition;
    } else {
        normalVarying = (modelMatrixUniform * vec4(normalAttribute, 0.0)).xyz;
        vec4 position = vec4(positionAttribute, 1.0);
        positionVarying = (modelMatrixUniform * position).xyz;
        gl_Position =  mvpMatrixUniform * position;
    }
}
