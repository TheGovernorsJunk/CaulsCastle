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

	void Update ()
	{
		int horizontal = (int)Input.GetAxis("Horizontal");
		if (horizontal != 0)
		{
			rb2D.velocity += new Vector2(horizontal * maxSpeed * Time.deltaTime, 0);
		}

		if (Input.GetKeyDown(KeyCode.UpArrow) && OnGround)
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
