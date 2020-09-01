precision mediump float;

uniform vec4 topColorUniform;
uniform vec4 bottomColorUniform;

varying vec2 uvVarying;

void main() {
    gl_FragColor = mix(bottomColorUniform, topColorUniform, uvVarying.y);
}
