using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour
{
	public float maxSpeed = 10f;
	public float jumpForce = 5f;

	private Rigidbody2D rb2D;
	private Animator anim;
	private GroundCollision groundCollision;

	void Awake()
	{
		rb2D = GetComponent<Rigidbody2D>();
		anim = GetComponent<Animator>();
		groundCollision = GetComponentInChildren<GroundCollision>();
	}

	void FixedUpdate ()
	{
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rb2D.velocity = new Vector2(horizontal * maxSpeed, rb2D.velocity.y);
		anim.SetFloat("speed", Mathf.Abs(horizontal));

		//if (Mathf.Abs(horizontal) == 1)
		//	transform.localScale = new Vector3(horizontal * transform.localScale.x, transform.localScale.y, transform.localScale.z);

		bool onGround = groundCollision.OnGround;
		anim.SetBool("jump", !onGround);

		int vertical = (int)Input.GetAxisRaw("Vertical");
		if ((vertical == 1) && onGround)
		{
			Vector2 vel = rb2D.velocity;
			vel.y = 0;
			rb2D.velocity = vel;
			rb2D.AddForce(new Vector2(0, jumpForce), ForceMode2D.Impulse);
		}
	}
}
