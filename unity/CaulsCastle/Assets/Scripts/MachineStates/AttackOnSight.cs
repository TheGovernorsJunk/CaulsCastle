﻿using UnityEngine;
using System.Collections;
using System.Linq;

public class AttackOnSight : State
{
	static AttackOnSight mSingleton;

	public static State GetInstance()
	{
		if (mSingleton != null)
		{
			return mSingleton;
		}
		mSingleton = new AttackOnSight();
		return mSingleton;
	}

	AttackOnSight() {}

	public override State Update(GameObject avatar)
	{
		GameObject target = avatar.GetComponent<LockOn>().Target;
		if (target == null) return DetectionState.GetInstance();

		float distance = avatar.GetComponent<SimpleAI>().AttackDistance;
		if ((target.transform.position - avatar.transform.position).sqrMagnitude <= distance * distance)
		{
			avatar.GetComponent<MobMotion>().PendingAttack = true;
		}

		MobMotion motion = avatar.GetComponent<MobMotion>();
		motion.PendingMovement = (target.transform.position - avatar.transform.position).normalized * 0.5f;
		return null;
	}
}
