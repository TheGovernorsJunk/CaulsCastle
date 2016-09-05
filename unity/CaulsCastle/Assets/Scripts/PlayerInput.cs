using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	private Movement m_movement;

	void Start()
	{
		m_movement = GetComponent<Movement>();
	}
	
	void Update ()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		Vector2 input = new Vector2(x, y);
		m_movement.Heading = input;
		m_movement.Speed = input.magnitude * m_movement.MaxSpeed;
	}
}
