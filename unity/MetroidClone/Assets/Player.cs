using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour
{
	public float maxSpeed = 100f;
	public float jumpDistance = 5f;

	private Rigidbody2D rb2D;
	private Animator anim;

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
			rb2D.velocity += new Vector2(0, jumpDistance);
		}
	}

	bool OnGround
	{
		get
		{
			return rb2D.velocity.y < float.Epsilon && rb2D.velocity.y > -float.Epsilon;
		}
	}
}
