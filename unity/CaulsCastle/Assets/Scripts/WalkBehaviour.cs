using UnityEngine;

public class WalkBehaviour : StateMachineBehaviour
{
	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");
	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");
	static float NormalThreshold = 0.01f;

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		Vector2 movement = animator.GetComponent<MobMotion>().Movement;
		animator.SetFloat(MovX, movement.x);
		animator.SetFloat(MovY, movement.y);
		if (Mathf.Abs(movement.x) >= NormalThreshold || Mathf.Abs(movement.y) >= NormalThreshold)
		{
			Vector2 normal = movement.normalized;
			animator.SetFloat(NormalX, normal.x);
			animator.SetFloat(NormalY, normal.y);
		}
	}
}
