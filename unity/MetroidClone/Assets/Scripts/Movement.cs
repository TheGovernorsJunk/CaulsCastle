using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	public float maxSpeed = 5f;
	public Rigidbody2D rigidbody;

	void FixedUpdate ()
	{
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rigidbody.velocity = new Vector2(
			horizontal * maxSpeed * Time.deltaTime,
			rigidbody.velocity.y);
	}
}
