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

	public void Update(GameObject avatar)
	{
		LockOn lockOn = avatar.GetComponent<LockOn>();
		if (lockOn.Target) return;

		SimpleAI props = avatar.GetComponent<SimpleAI>();
		Vector3 position = avatar.transform.position;
		Collider2D[] collisions = Physics2D.OverlapCircleAll(position, props.SenseRadius, props.SenseMask);
		Collider2D playerCollider = collisions.FirstOrDefault(c => c.tag == "Player");
		if (playerCollider) lockOn.Trigger(playerCollider.transform.position - position);
	}
}
