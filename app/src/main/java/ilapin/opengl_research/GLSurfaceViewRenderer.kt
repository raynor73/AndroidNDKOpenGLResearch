package ilapin.opengl_research

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.util.Log
import ilapin.opengl_research.MainActivity.Companion.LOG_TAG
import java.nio.ByteBuffer
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

    fun loadBinaryFileFromAssets(path: String): ByteArray {
        val inputStream = context.assets.open(path)
        val fileContent = inputStream.readBytes()
        inputStream.close()
        return fileContent
    }

    fun loadBitmapFileFromAssets(path: String): BitmapInfo {
        val inputStream = context.assets.open(path)
        val bitmap = BitmapFactory.decodeStream(inputStream)
        val byteBuffer = ByteBuffer.allocateDirect(bitmap.width * bitmap.height * 4);
        bitmap.copyPixelsToBuffer(byteBuffer)
        inputStream.close()
        return BitmapInfo(byteBuffer.array(), bitmap.width, bitmap.height)
    }

    @SuppressLint("LongLogTag")
    fun putMessage(message: Any) {
        when (message) {
            is TouchEvent -> gameWrapperPutTouchEventIntoQueue(message)
            else -> Log.e(LOG_TAG, "Unexpected message type ${message.javaClass.simpleName}")
        }
    }

    private external fun gameWrapperInit(displayDensityFactor: Float)
    private external fun gameWrapperPutTouchEventIntoQueue(touchEvent: TouchEvent)
    private external fun gameWrapperOnDrawFrame()
    private external fun gameWrapperOnSurfaceChanged(width: Int, height: Int)
    private external fun gameWrapperOnSurfaceCreated()
}