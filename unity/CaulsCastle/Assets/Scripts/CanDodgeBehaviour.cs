using UnityEngine;

public class CanDodgeBehaviour : StateMachineBehaviour
{
	static int Dodge = Animator.StringToHash("Dodge");

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		MobMotion motion = animator.GetComponent<MobMotion>();
		if (motion.PendingDodge)
		{
			animator.SetTrigger(Dodge);
			motion.PendingDodge = false;
		}
	}
}
