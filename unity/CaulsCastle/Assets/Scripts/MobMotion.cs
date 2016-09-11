using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Animator))]
[RequireComponent(typeof(Rigidbody2D))]

public class MobMotion : MonoBehaviour
{
	Animator mAnimator;
	Rigidbody2D mRigidbody;

	public Vector2 Movement { get; set; }
	public bool Attack { get; set; }

	void Awake()
	{
		mAnimator = GetComponent<Animator>();
		mRigidbody = GetComponent<Rigidbody2D>();
	}

	void OnAnimatorMove()
	{
		mRigidbody.velocity = mAnimator.deltaPosition / Time.deltaTime;
	}
}
