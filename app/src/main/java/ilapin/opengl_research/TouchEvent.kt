package ilapin.opengl_research

/**
 * @author igorlapin on 25/07/2020.
 */
class TouchEvent(
    val id: Int,
    val action: Action,
    val x: Int,
    val y: Int

) {
    enum class Action {
        DOWN, UP, CANCEL, MOVE
    }
}