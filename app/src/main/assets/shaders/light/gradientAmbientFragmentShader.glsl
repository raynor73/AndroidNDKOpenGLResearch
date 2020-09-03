precision mediump float;

uniform vec4 topColorUniform;
uniform vec4 bottomColorUniform;

uniform vec3 topPositionUniform;
uniform vec3 bottomPositionUniform;

uniform vec3 ambientColorUniform;

void main() {
    
    gl_FragColor = mix(bottomColorUniform, topColorUniform, uvVarying.y);
}
