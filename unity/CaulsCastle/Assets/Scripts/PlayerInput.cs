using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	public GameObject Avatar;
	public float MovementThreshold = 0.3f;

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
		if (Mathf.Abs(x) > MovementThreshold || Mathf.Abs(y) > MovementThreshold)
		{
			Vector2 input = new Vector2(x, y);
			m_movement.Velocity = input * m_movement.MaxSpeed;
		}
		else
		{
			m_movement.Speed = 0;
		}

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
