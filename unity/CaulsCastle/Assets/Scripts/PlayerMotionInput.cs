using UnityEngine;
using System.Collections;

public class PlayerMotionInput : MonoBehaviour
{
	public GameObject Avatar;

	MobMotion mMobMotion;

	void Awake()
	{
		mMobMotion = Avatar.GetComponent<MobMotion>();
	}

	bool attackDown = false;
	void Update()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		mMobMotion.Movement = new Vector2(x, y);

		float attackInput = Input.GetAxisRaw("LightAttack");
		if (attackInput > 0 && !attackDown)
		{
			mMobMotion.Attack = attackDown = true;
		}
		else if (attackInput == 0)
		{
			attackDown = false;
		}
	}
}
