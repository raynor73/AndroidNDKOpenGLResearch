//
// Created by Igor Lapin on 24/07/2020.
//

#include <GLES2/gl2.h>
#include <exception>
#include <sstream>
#include "OpenGLTexturesRepository.h"

void OpenGLTexturesRepository::createTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data
) {
    createTexture(name, width, height, data, false);
}

void OpenGLTexturesRepository::createTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data,
        bool isBeingRestored
) {
    if (m_textures.count(name) > 0) {
        std::stringstream ss;
        ss << "Texture " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    texturesCreationParams += TextureCreationParams.FromData(
            name,
            width,
            height,
            IntArray(data.size) { i -> data[i] }
    )

    GLES20.glGenTextures(1, tmpIntArray, 0)
    val texture = tmpIntArray[0]

    val bitmap = Bitmap.createBitmap(data, width, height, Bitmap.Config.ARGB_8888)

    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture)
    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST)
    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST)
    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)

    GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0)

    bitmap.recycle()

    textures[name] = TextureInfo(texture, width, height)

    GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D)
    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)

    openGLErrorDetector.dispatchOpenGLErrors("createTexture from data")
}
