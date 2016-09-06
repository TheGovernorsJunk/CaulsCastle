using UnityEngine;
using System.Collections;

public class MobAnimator : MonoBehaviour {

	static int UP = Animator.StringToHash("up");
	static int DOWN = Animator.StringToHash("down");
	static int LEFT = Animator.StringToHash("left");
	static int RIGHT = Animator.StringToHash("right");
	static int SPEED = Animator.StringToHash("speed");

	Movement m_movement;
	Animator m_animator;

	void Awake ()
	{
		m_movement = GetComponent<Movement>();
		m_animator = GetComponent<Animator>();
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
