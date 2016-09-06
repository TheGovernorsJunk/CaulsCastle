using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	public GameObject Avatar;

	static int ATTACK = Animator.StringToHash("attack");

	Movement m_movement;
	Animator m_animator;

	void Awake()
	{
		m_movement = Avatar.GetComponent<Movement>();
		m_animator = Avatar.GetComponent<Animator>();
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

		float attackEvent = Input.GetAxisRaw("Fire1");
		if (attackEvent > 0 && !attacking)
		{
			m_animator.SetTrigger(ATTACK);
			attacking = true;
		}
		else if (attackEvent == 0)
		{
			attacking = false;
		}
	}
}
