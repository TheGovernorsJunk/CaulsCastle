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
	
	void Update()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		mMobMotion.Movement = new Vector2(x, y);
	}
}
