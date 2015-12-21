using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour
{
	public float maxSpeed = 100f;

	private Rigidbody2D rb2D;

	void Awake()
	{
		rb2D = GetComponent<Rigidbody2D>();
	}

	void FixedUpdate ()
	{
		int horizontal = (int)Input.GetAxisRaw("Horizontal");
		rb2D.velocity -= new Vector2(rb2D.velocity.x, 0);
		rb2D.velocity += new Vector2(horizontal * maxSpeed, 0);

		int vertical = (int)Input.GetAxisRaw("Vertical");
		if (vertical == 1 && OnGround)
		{
			rb2D.velocity += new Vector2(0, 5);
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
