using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	private Rigidbody2D m_rigidbody;
	private Vector2 m_direction;

	void Start()
	{
		m_rigidbody = GetComponent<Rigidbody2D>();
	}

	void Update()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		m_direction = new Vector2(x, y);
	}

	void FixedUpdate()
	{
		m_rigidbody.velocity = m_direction * 5;
	}
}
