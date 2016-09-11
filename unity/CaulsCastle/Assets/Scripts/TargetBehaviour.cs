using UnityEngine;
using System.Collections;

public class TargetBehaviour : StateMachineBehaviour
{
	static int HasTarget = Animator.StringToHash("HasTarget");
	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		GameObject target = animator.GetComponent<LockOn>().Target;
		if (target)
		{
			Transform transform = animator.transform;
			Vector3 heading = target.transform.position - transform.position;
			heading.Normalize();
			animator.SetFloat(NormalX, heading.x);
			animator.SetFloat(NormalY, heading.y);
		}
		else
		{
			animator.SetBool(HasTarget, false);
		}
	}
}
