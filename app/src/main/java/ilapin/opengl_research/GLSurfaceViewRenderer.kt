package ilapin.opengl_research

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.BitmapFactory
import android.media.MediaPlayer
import android.opengl.GLSurfaceView
import android.util.Log
import ilapin.opengl_research.MainActivity.Companion.LOG_TAG
import java.io.File
import java.nio.ByteBuffer
import java.nio.charset.Charset
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * @author igorlapin on 03/06/2020.
 */
class GLSurfaceViewRenderer(private val context: Context, displayDensityFactor: Float) : GLSurfaceView.Renderer {

    private val musicPlayer = MediaPlayer()

    init {
        gameWrapperInit(displayDensityFactor)

        musicPlayer.setOnPreparedListener { musicPlayer.start() }
        musicPlayer.setOnCompletionListener {  }
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

    fun isFileExistsInAssets(path: String): Boolean {
        val file = File(path)
        return context.assets.list(file.parent ?: "")?.contains(file.name) ?: false
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

    fun isFileExistsInPrivateStorage(path: String): Boolean {
        return File(context.filesDir, path).exists()
    }

    fun loadTextFileFromPrivateStorage(path: String): String {
        val inputStream = context.openFileInput(path)
        val fileContent = inputStream.readBytes().toString(Charset.defaultCharset())
        inputStream.close()
        return fileContent
    }

    fun saveTextToFileInPrivateStorage(path: String, text: String) {
        val outputStream = context.openFileOutput(path, Context.MODE_PRIVATE)
        outputStream.write(text.toByteArray(Charset.defaultCharset()))
        outputStream.close()
    }

    fun loadBinaryFileFromPrivateStorage(path: String): ByteArray {
        val inputStream = context.openFileInput(path)
        val fileContent = inputStream.readBytes()
        inputStream.close()
        return fileContent
    }

    fun saveDataToFileInPrivateStorage(path: String, data: ByteArray) {
        val outputStream = context.openFileOutput(path, Context.MODE_PRIVATE)
        outputStream.write(data)
        outputStream.close()
    }

    fun deleteFileFromPrivateStorage(path: String) {
        File(context.filesDir, path).apply {
            if (exists()) {
                delete()
            }
        }
    }

    fun playMusic(path: String) {
        musicPlayer.reset()

        val fileDescriptor = context.assets.openFd(path)
        musicPlayer.setDataSource(fileDescriptor.fileDescriptor, fileDescriptor.startOffset, fileDescriptor.length)
        fileDescriptor.close()

        musicPlayer.prepareAsync()
    }

    fun pauseMusic() {
        if (musicPlayer.isPlaying) {
            musicPlayer.pause()
        }
    }

    fun resumeMusic() {
        musicPlayer.start()
    }

    fun stopMusic() {
        musicPlayer.reset()
    }

    fun setMusicGain(gain: Float) {
        musicPlayer.setVolume(gain, gain)
    }

    @SuppressLint("LongLogTag")
    fun putMessage(message: Any) {
        when (message) {
            is AppState ->
                if (message.isAppInForeground)
                    gameWrapperReportAppInForeground()
                else
                    gameWrapperReportAppInBackground()

            is TouchEvent -> gameWrapperPutTouchEventIntoQueue(message)

            else -> Log.e(LOG_TAG, "Unexpected message type ${message.javaClass.simpleName}")
        }
    }

    private external fun gameWrapperInit(displayDensityFactor: Float)
    private external fun gameWrapperPutTouchEventIntoQueue(touchEvent: TouchEvent)
    private external fun gameWrapperReportAppInForeground()
    private external fun gameWrapperReportAppInBackground()
    private external fun gameWrapperReportMusicCompletion()
    private external fun gameWrapperOnDrawFrame()
    private external fun gameWrapperOnSurfaceChanged(width: Int, height: Int)
    private external fun gameWrapperOnSurfaceCreated()
}