using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour
{

	public float maxSpeed = 100f;

	private Rigidbody2D rigidbody;

	void Start()
	{
		rigidbody = GetComponent<Rigidbody2D>();
	}

	void FixedUpdate ()
	{
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rigidbody.velocity = new Vector2(
			horizontal * maxSpeed * Time.deltaTime,
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
