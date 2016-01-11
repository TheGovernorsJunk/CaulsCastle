using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour
{

	public float walkSpeed = 100f;
	public float runSpeed = 175f;

	private Rigidbody2D rigidbody;

	void Start()
	{
		rigidbody = GetComponent<Rigidbody2D>();
	}

	void FixedUpdate ()
	{
		int isRunning = (int)Input.GetAxisRaw("Fire4");
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rigidbody.velocity = new Vector2(
			horizontal * (isRunning == 1 ? runSpeed : walkSpeed) * Time.deltaTime,
			rigidbody.velocity.y);

		if (horizontal != 0)
		{
			BroadcastMessage("OnMove", horizontal);
		}
		else
		{
			BroadcastMessage("OnIdle");
		}
	}
}
