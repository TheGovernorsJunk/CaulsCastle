using UnityEngine;

public class DodgeBehaviour : StateMachineBehaviour
{
	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");

	override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		Vector2 movement = animator.GetComponent<MobMotion>().Movement;
		movement.Normalize();
		animator.SetFloat(MovX, movement.x);
		animator.SetFloat(MovY, movement.y);
	}
}
