using UnityEngine;
using System.Collections;

public class PlayerMotionInput : MonoBehaviour
{
	public GameObject Avatar;

	MobMotion mMobMotion;
	LockOn mLockOn;
	Animator mAnimator;

	static float LockInfluenceThreshold = 0.3f;

	void Awake()
	{
		mMobMotion = Avatar.GetComponent<MobMotion>();
		mLockOn = Avatar.GetComponent<LockOn>();
		mAnimator = Avatar.GetComponent<Animator>();
	}

	bool attackDown = false;
	bool lockDown = false;
	bool dodgeDown = false;
	void Update()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		mMobMotion.PendingMovement = new Vector2(x, y);

		float attackInput = Input.GetAxisRaw("LightAttack");
		if (attackInput > 0 && !attackDown)
		{
			mMobMotion.PendingAttack = attackDown = true;
		}
		else if (attackInput == 0)
		{
			attackDown = false;
		}

		float lockInput = Input.GetAxisRaw("Lock");
		if (lockInput > 0 && !lockDown)
		{
			float lockX = Input.GetAxisRaw("U");
			float lockY = Input.GetAxisRaw("V");
			Vector2 lockDirection = Mathf.Abs(lockX) > LockInfluenceThreshold || Mathf.Abs(lockY) > LockInfluenceThreshold
				? new Vector2(lockX, lockY)
				: mMobMotion.Heading;
			mLockOn.Trigger(lockDirection);
			lockDown = true;
		}
		else if (lockInput == 0)
		{
			lockDown = false;
		}

		float dodgeInput = Input.GetAxisRaw("Dodge");
		if (dodgeInput > 0 && !dodgeDown)
		{
			mMobMotion.PendingDodge = dodgeDown = true;
		}
		else if (dodgeInput == 0)
		{
			dodgeDown = false;
		}
	}
}
