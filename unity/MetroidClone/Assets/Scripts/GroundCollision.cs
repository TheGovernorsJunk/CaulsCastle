using UnityEngine;
using System.Collections;

public class GroundCollision : MonoBehaviour {

	public LayerMask groundMask;

	public bool OnGround
	{
		get
		{
			return groundCheck.IsTouchingLayers(groundMask);
		}
	}

	private CircleCollider2D groundCheck;

	void Awake ()
	{
		groundCheck = GetComponent<CircleCollider2D>();
	}
}
