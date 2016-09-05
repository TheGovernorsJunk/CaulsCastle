using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	public float MaxSpeed = 5.0f;
	public Vector2 Heading
	{
		get { return m_heading; }
		set { m_heading = value.normalized; }
	}
	public float Speed { get; set; }

	private Rigidbody2D m_rigidbody;
	private Vector2 m_heading;

	void Start()
	{
		m_rigidbody = GetComponent<Rigidbody2D>();
	}

	void FixedUpdate()
	{
		m_rigidbody.velocity = m_heading * Mathf.Min(MaxSpeed, Speed);
	}
}
