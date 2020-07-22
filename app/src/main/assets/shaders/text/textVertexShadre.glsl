attribute vec3 positionAttribute;
attribute vec2 uvAttribute;

uniform mat4 mvpMatrixUniform;

varying vec2 uvVarying;

void main() {
    uvVarying = uvAttribute;

    gl_Position = mvpMatrixUniform * vec4(positionAttribute, 1.0);
}
