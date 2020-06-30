package ilapin.opengl_research

import android.content.Context
import android.opengl.GLSurfaceView
import java.nio.charset.Charset
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * @author igorlapin on 03/06/2020.
 */
class GLSurfaceViewRenderer(private val context: Context, displayDensityFactor: Float) : GLSurfaceView.Renderer {

    init {
        gameWrapperInit(displayDensityFactor)
    }

    override fun onDrawFrame(gl: GL10?) {
        gameWrapperOnDrawFrame()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        gameWrapperOnSurfaceChanged(width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        gameWrapperOnSurfaceCreated()
    }

    fun loadTextFileFromAssets(path: String): String {
        val inputStream = context.assets.open(path)
        val fileContent = inputStream.readBytes().toString(Charset.defaultCharset())
        inputStream.close()
        return fileContent
    }

    private external fun gameWrapperInit(displayDensityFactor: Float)
    private external fun gameWrapperOnDrawFrame()
    private external fun gameWrapperOnSurfaceChanged(width: Int, height: Int)
    private external fun gameWrapperOnSurfaceCreated()
}