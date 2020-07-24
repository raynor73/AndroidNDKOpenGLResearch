precision mediump float;

uniform sampler2D textureUniform;
uniform vec4 diffuseColorUniform;

varying vec2 uvVarying;

void main() {
    gl_FragColor = texture2D(textureUniform, uvVarying) * diffuseColorUniform;
}
