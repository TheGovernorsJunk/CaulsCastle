using UnityEngine;
using System.Collections;

public class MobAnimator : MonoBehaviour {

	int UP = Animator.StringToHash("up");
	int DOWN = Animator.StringToHash("down");
	int LEFT = Animator.StringToHash("left");
	int RIGHT = Animator.StringToHash("right");
	int SPEED = Animator.StringToHash("speed");

	Movement m_movement;
	Animator m_animator;

	void Awake ()
	{
		m_movement = GetComponentInChildren<Movement>();
		m_animator = GetComponentInChildren<Animator>();
	}
	
	void FixedUpdate ()
	{
		Vector2 heading = m_movement.Heading;
		float x = heading.x;
		float y = heading.y;
		if (x != 0 || y != 0)
		{
			if (Mathf.Abs(x) > Mathf.Abs(y))
			{
				m_animator.SetTrigger(x > 0 ? RIGHT : LEFT);
			}
			else
			{
				m_animator.SetTrigger(y > 0 ? UP : DOWN);
			}
			m_animator.SetInteger(SPEED, 1);
		}
		else
		{
			m_animator.SetInteger(SPEED, 0);
		}
	}
}
