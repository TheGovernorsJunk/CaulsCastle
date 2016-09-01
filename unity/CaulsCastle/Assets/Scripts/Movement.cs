using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	public float Speed = 5.0f;

	private Rigidbody2D m_rigidbody;
	private Vector2 m_heading;

	void Start()
	{
		m_rigidbody = GetComponent<Rigidbody2D>();
	}

	public void SetHeading(Vector2 heading)
	{
		m_heading = heading.normalized;
	}

	void FixedUpdate()
	{
		m_rigidbody.velocity = m_heading * Speed;
	}
}
