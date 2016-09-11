using UnityEngine;

public class MoveBehaviour : StateMachineBehaviour
{
	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");
	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");
	static int Attack = Animator.StringToHash("Attack");
	static float NormalThreshold = 0.01f;

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		MobMotion motion = animator.GetComponent<MobMotion>();
		Vector2 movement = motion.Movement;
		animator.SetFloat(MovX, movement.x);
		animator.SetFloat(MovY, movement.y);

		if (motion.Attack)
		{
			animator.SetTrigger(Attack);
			motion.Attack = false;
		}
	}
}
