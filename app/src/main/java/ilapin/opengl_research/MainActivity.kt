package ilapin.opengl_research

import android.annotation.SuppressLint
import android.opengl.GLSurfaceView
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private lateinit var renderer: GLSurfaceViewRenderer

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        renderer = GLSurfaceViewRenderer(this, resources.displayMetrics.density)

        val glView = GLSurfaceView(this)
        glView.setOnTouchListener { _, event ->
            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN, MotionEvent.ACTION_POINTER_DOWN -> {
                    renderer.putMessage(TouchEvent(
                        event.getPointerId(event.actionIndex),
                        TouchEvent.DOWN,
                        event.getX(event.actionIndex).toInt(),
                        glView.height - event.getY(event.actionIndex).toInt()
                    ))
                }

                MotionEvent.ACTION_MOVE -> {
                    repeat(event.pointerCount) { i ->
                        renderer.putMessage(TouchEvent(
                            event.getPointerId(i),
                            TouchEvent.MOVE,
                            event.getX(i).toInt(),
                            glView.height - event.getY(i).toInt()
                        ))
                    }
                }

                MotionEvent.ACTION_UP, MotionEvent.ACTION_POINTER_UP -> {
                    renderer.putMessage(TouchEvent(
                        event.getPointerId(event.actionIndex),
                        TouchEvent.UP,
                        event.getX(event.actionIndex).toInt(),
                        glView.height - event.getY(event.actionIndex).toInt()
                    ))
                }

                else -> {
                    renderer.putMessage(TouchEvent(
                        event.getPointerId(event.actionIndex),
                        TouchEvent.CANCEL,
                        event.getX(event.actionIndex).toInt(),
                        glView.height - event.getY(event.actionIndex).toInt()
                    ))
                }
            }

            true
        }
        glView.setEGLContextClientVersion(2)
        glView.setRenderer(renderer)
        glView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        containerLayout.addView(glView, 0)
    }

    override fun onResume() {
        super.onResume()

        hideControls()
    }

    private fun hideControls() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_LOW_PROFILE or
                    View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or
                    View.SYSTEM_UI_FLAG_FULLSCREEN or
                    View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or
                    View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION or
                    View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
    }

    companion object {

        const val LOG_TAG = "AndroidNDKOpenGLResearch"

        init {
            System.loadLibrary("ode")
            System.loadLibrary("freetype")
            System.loadLibrary("assimp")
            System.loadLibrary("native-lib")
        }
    }
}
