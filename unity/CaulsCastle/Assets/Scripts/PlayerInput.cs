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
		m_movement.SetHeading(new Vector2(x, y));
	}
}
