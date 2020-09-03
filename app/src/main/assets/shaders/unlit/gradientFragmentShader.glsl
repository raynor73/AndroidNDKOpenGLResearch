precision mediump float;

uniform vec4 topColorUniform;
uniform vec4 bottomColorUniform;

uniform vec3 topPoint;
uniform vec3 bottomPoint;

varying vec3 positionVarying;

void main() {
    gl_FragColor = mix(bottomColorUniform, topColorUniform, smoothstep(topPoint.y, bottomPoint.y, positionVarying.y));
}
