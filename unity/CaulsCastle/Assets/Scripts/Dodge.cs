using UnityEngine;
using System.Collections;

public class Dodge : MonoBehaviour
{

	static int DODGE = Animator.StringToHash("dodge");

	Animator mAnimator;

	void Awake()
	{
		mAnimator = GetComponent<Animator>();
	}

	public void Trigger()
	{
		if (!enabled) return;
		mAnimator.SetTrigger(DODGE);
	}
}
