﻿using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	int ATTACK = Animator.StringToHash("attack");

	Movement m_movement;
	Animator m_animator;

	void Start()
	{
		m_movement = GetComponentInChildren<Movement>();
		m_animator = GetComponentInChildren<Animator>();
	}

	bool attacking = false;
	void Update ()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float y = Input.GetAxisRaw("Vertical");
		Vector2 input = new Vector2(x, y);
		m_movement.Heading = input;
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
