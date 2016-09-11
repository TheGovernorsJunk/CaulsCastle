using UnityEngine;
using System.Collections;

public class CanAttackBehaviour : StateMachineBehaviour
{
	static int Attack = Animator.StringToHash("Attack");

	override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
	{
		MobMotion motion = animator.GetComponent<MobMotion>();
		if (motion.PendingAttack)
		{
			animator.SetTrigger(Attack);
			motion.PendingAttack = false;
		}
	}
}
