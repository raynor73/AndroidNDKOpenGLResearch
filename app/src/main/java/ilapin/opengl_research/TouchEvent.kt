package ilapin.opengl_research

/**
 * @author igorlapin on 25/07/2020.
 */
class TouchEvent(
    val id: Int,
    val action: Int,
    val x: Int,
    val y: Int
) {
    companion object {
        const val DOWN = 0
        const val MOVE = 1
        const val UP = 2
        const val CANCEL = 3
    }
}