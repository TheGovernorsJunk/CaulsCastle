using UnityEngine;
using System.Collections;

public class PlayerMotionInput : MonoBehaviour
{
	public GameObject Avatar;

	static int MovX = Animator.StringToHash("MovX");
	static int MovY = Animator.StringToHash("MovY");
	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");
	static float NormalThreshold = 0.01f;
	Animator mAnimator;

	void Awake()
	{
		mAnimator = Avatar.GetComponent<Animator>();
	}
	
	void Update()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");

		mAnimator.SetFloat(MovX, x);
		mAnimator.SetFloat(MovY, y);

		if (Mathf.Abs(x) >= NormalThreshold || Mathf.Abs(y) >= NormalThreshold)
		{
			Vector2 normal = new Vector2(x, y);
			normal.Normalize();
			mAnimator.SetFloat(NormalX, normal.x);
			mAnimator.SetFloat(NormalY, normal.y);
		}
	}
}
