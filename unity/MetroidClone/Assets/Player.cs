using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour
{
	public float maxSpeed = 100f;
	public float jumpDistance = 5f;
	public Transform groundCheck;
	public LayerMask groundMask;

	private Rigidbody2D rb2D;
	private Animator anim;

	bool OnGround
	{
		get
		{
			return Physics2D.OverlapPoint(groundCheck.position, groundMask);
		}
	}

	void Awake()
	{
		rb2D = GetComponent<Rigidbody2D>();
		anim = GetComponent<Animator>();
	}

	void FixedUpdate ()
	{
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rb2D.velocity = new Vector2(horizontal * maxSpeed, rb2D.velocity.y);
		anim.SetFloat("speed", Mathf.Abs(horizontal));

		int vertical = (int)Input.GetAxisRaw("Vertical");
		if (vertical == 1 && OnGround)
		{
			rb2D.velocity = new Vector2(rb2D.velocity.x, jumpDistance);
		}
	}
}
