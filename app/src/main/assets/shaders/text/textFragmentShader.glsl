precision mediump float;

uniform sampler2D textureUniform;
uniform vec4 diffuseColorUniform;

varying vec2 uvVarying;

void main() {
    gl_FragColor = vec4(diffuseColorUniform.xyz, texture2D(textureUniform, uvVarying).r * diffuseColorUniform.w);
}
