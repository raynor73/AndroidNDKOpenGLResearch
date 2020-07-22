precision mediump float;

uniform vec4 diffuseColorUniform;

varying vec2 uvVarying;

void main() {
    gl_FragColor = diffuseColorUniform;
}
