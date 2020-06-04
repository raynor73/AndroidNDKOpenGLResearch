package ilapin.opengl_research

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * @author igorlapin on 03/06/2020.
 */
class GLSurfaceViewRenderer : GLSurfaceView.Renderer {

    override fun onDrawFrame(gl: GL10?) {
        gameWrapperOnDrawFrame()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        gameWrapperOnSurfaceChanged(width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        gameWrapperOnSurfaceCreated()
    }

    private external fun gameWrapperOnDrawFrame()
    private external fun gameWrapperOnSurfaceChanged(width: Int, height: Int)
    private external fun gameWrapperOnSurfaceCreated()
}