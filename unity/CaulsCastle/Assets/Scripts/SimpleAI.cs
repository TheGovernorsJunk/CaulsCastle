using UnityEngine;
using System.Collections;

[RequireComponent(typeof(LockOn))]
[RequireComponent(typeof(MobMotion))]

public class SimpleAI : MonoBehaviour
{
	public float SenseRadius = 5f;
	public LayerMask SenseMask = -1;
	public float AttackDistance = 1.5f;

	StateMachine mStateMachine;

	void Awake()
	{
		mStateMachine = new StateMachine(DetectionState.GetInstance());
	}
	
	void Update ()
	{
		mStateMachine.Update(gameObject);
	}
}
