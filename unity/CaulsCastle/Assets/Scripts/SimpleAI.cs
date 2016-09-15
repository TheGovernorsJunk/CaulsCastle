using UnityEngine;
using System.Collections;

[RequireComponent(typeof(LockOn))]

public class SimpleAI : MonoBehaviour
{
	public float SenseRadius = 5f;
	public LayerMask SenseMask = -1;

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
