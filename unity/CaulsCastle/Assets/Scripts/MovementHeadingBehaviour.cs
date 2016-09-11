using UnityEngine;

public class MovementHeadingBehaviour : StateMachineBehaviour
{
	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");
	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");
	static float NormalThreshold = 0.01f;

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		float x = animator.GetFloat(MovX);
		float y = animator.GetFloat(MovY);
		if (Mathf.Abs(x) >= NormalThreshold || Mathf.Abs(y) >= NormalThreshold)
		{
			Vector2 normal = new Vector2(x, y);
			normal.Normalize();
			animator.SetFloat(NormalX, normal.x);
			animator.SetFloat(NormalY, normal.y);
		}
	}
}
