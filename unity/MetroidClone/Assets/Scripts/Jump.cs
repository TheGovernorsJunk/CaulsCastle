using UnityEngine;
using System.Collections;

public class Jump : MonoBehaviour
{
	public float jumpForce = 5f;

	private Rigidbody2D rigidbody;
	private GroundCheck groundCheck;

	void Start ()
	{
		rigidbody = GetComponent<Rigidbody2D>();
		groundCheck = GetComponentInChildren<GroundCheck>();
	}
	
	void FixedUpdate ()
	{
		int vertical = (int)Input.GetAxisRaw("Fire2");
		if ((vertical == 1) && groundCheck.Grounded)
		{
			Vector2 vel = rigidbody.velocity;
			vel.y = 0;
			rigidbody.velocity = vel;
			rigidbody.AddForce(new Vector2(0, jumpForce), ForceMode2D.Impulse);
		}
		BroadcastMessage("OnJump", !groundCheck.Grounded);
	}
}
