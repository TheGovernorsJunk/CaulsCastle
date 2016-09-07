using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	public float MaxSpeed = 5.0f;
	public Vector2 Heading
	{
		get { return m_heading; }
		set { m_heading = value.normalized; }
	}
	public float Speed
	{
		get { return m_speed; }
		set { m_speed = Mathf.Min(MaxSpeed, value); }
	}
	public Vector2 Velocity
	{
		get { return m_heading * m_speed; }
		set
		{
			float magnitude = value.magnitude;
			Speed = magnitude;
			m_heading = value / magnitude;
		}
	}

	Rigidbody2D m_rigidbody;
	Vector2 m_heading;
	float m_speed;

	void Awake()
	{
		m_rigidbody = GetComponent<Rigidbody2D>();
	}

	void FixedUpdate()
	{
		m_rigidbody.velocity = m_heading * m_speed;
	}

	void OnDisable()
	{
		Speed = 0;
		m_rigidbody.velocity = Vector2.zero;
	}
}
