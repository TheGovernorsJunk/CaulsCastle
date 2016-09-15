using UnityEngine;
using System.Collections;

[RequireComponent(typeof(LockOn))]

public class SimpleAI : MonoBehaviour
{
	public float SenseRadius = 5f;
	public LayerMask SenseMask = -1;

	LockOn mLockOn;
	StateMachine mStateMachine;

	void Awake()
	{
		mLockOn = GetComponent<LockOn>();
		mStateMachine = new StateMachine(AttackOnSight.GetInstance());
	}
	
	void Update ()
	{
		mStateMachine.Update(gameObject);
	}
}
