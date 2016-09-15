using UnityEngine;
using System.Collections;
using System.Linq;

[RequireComponent(typeof(LockOn))]

public class SimpleAI : MonoBehaviour
{
	public float SenseRadius = 5f;
	public LayerMask SenseMask = -1;

	LockOn mLockOn;

	void Awake()
	{
		mLockOn = GetComponent<LockOn>();
	}
	
	void Update ()
	{
		if (mLockOn.Target) return;

		Vector3 position = transform.position;
		Collider2D[] collisions = Physics2D.OverlapCircleAll(position, SenseRadius, SenseMask);
		Collider2D playerCollider = collisions.FirstOrDefault(c => c.tag == "Player");
		if (playerCollider) mLockOn.Trigger(playerCollider.transform.position - position);
	}
}
