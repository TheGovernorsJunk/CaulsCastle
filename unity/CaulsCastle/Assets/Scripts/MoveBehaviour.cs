using UnityEngine;

public class MoveBehaviour : StateMachineBehaviour
{
	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		MobMotion motion = animator.GetComponent<MobMotion>();
		Vector2 movement = motion.PendingMovement;
		animator.SetFloat(MovX, movement.x);
		animator.SetFloat(MovY, movement.y);
	}
}
