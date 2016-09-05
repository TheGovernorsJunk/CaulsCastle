using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	public float Speed = 5.0f;
	public Vector2 Heading
	{
		get { return m_heading; }
		set { m_heading = value.normalized; }
	}

	private Rigidbody2D m_rigidbody;
	private Vector2 m_heading;
	private Animator m_animator;

	private int UP = Animator.StringToHash("up");
	private int DOWN = Animator.StringToHash("down");
	private int LEFT = Animator.StringToHash("left");
	private int RIGHT = Animator.StringToHash("right");
	private int SPEED = Animator.StringToHash("speed");

	void Start()
	{
		m_rigidbody = GetComponent<Rigidbody2D>();
		m_animator = GetComponent<Animator>();
	}

	void FixedUpdate()
	{
		m_rigidbody.velocity = m_heading * Speed;

		float x = m_heading.x;
		float y = m_heading.y;
		if (x != 0 || y != 0)
		{
			if (Mathf.Abs(x) > Mathf.Abs(y))
			{
				m_animator.SetTrigger(x > 0 ? RIGHT : LEFT);
			}
			else
			{
				m_animator.SetTrigger(y > 0 ? UP : DOWN);
			}
			m_animator.SetInteger(SPEED, 1);
		}
		else
		{
			m_animator.SetInteger(SPEED, 0);
		}
	}
}
