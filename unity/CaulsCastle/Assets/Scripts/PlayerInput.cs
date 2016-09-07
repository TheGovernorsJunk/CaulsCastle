using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	public GameObject Avatar;

	Movement m_movement;
	LightAttack m_light_attack;

	void Awake()
	{
		m_movement = Avatar.GetComponent<Movement>();
		m_light_attack = Avatar.GetComponent<LightAttack>();
	}

	bool attacking = false;
	void Update ()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		Vector2 input = new Vector2(x, y);
		if (x != 0 || y != 0)
		{
			m_movement.Heading = input;
		}
		m_movement.Speed = input.magnitude * m_movement.MaxSpeed;

		float attackEvent = Input.GetAxisRaw("LightAttack");
		if (attackEvent > 0 && !attacking)
		{
			m_light_attack.Trigger();
			attacking = true;
		}
		else if (attackEvent == 0)
		{
			attacking = false;
		}
	}
}
