using UnityEngine;
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
		LockOn lockOn = avatar.GetComponent<LockOn>();
		if (lockOn.Target == null) return DetectionState.GetInstance();
		return null;
	}
}
