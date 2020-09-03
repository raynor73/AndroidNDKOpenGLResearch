precision mediump float;

uniform sampler2D textureUniform;
uniform vec4 diffuseColorUniform;
uniform bool useDiffuseColorUniform;
uniform vec4 topColorUniform;
uniform vec4 bottomColorUniform;
uniform bool isGradientUniform;

uniform vec3 topPointUniform;
uniform vec3 bottomPointUniform;

varying vec3 positionVarying;

varying vec2 uvVarying;

void main() {
    if (isGradientUniform) {
        gl_FragColor = mix(
            topColorUniform,
            bottomColorUniform,
            smoothstep(topPointUniform.y, bottomPointUniform.y, positionVarying.y)
        );
    } else {
        if (useDiffuseColorUniform) {
            gl_FragColor = diffuseColorUniform;
        } else {
            gl_FragColor = texture2D(textureUniform, uvVarying);
        }
    }
}
