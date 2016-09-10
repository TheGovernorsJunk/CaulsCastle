using UnityEngine;
using System.Collections;

public class MobAnimator : MonoBehaviour {

	static int UP = Animator.StringToHash("up");
	static int DOWN = Animator.StringToHash("down");
	static int LEFT = Animator.StringToHash("left");
	static int RIGHT = Animator.StringToHash("right");
	static int SPEED = Animator.StringToHash("speed");
	static int DODGING = Animator.StringToHash("dodging");

	Movement m_movement;
	LockOn m_lock_on;
	Animator m_animator;

	void Awake ()
	{
		m_movement = GetComponent<Movement>();
		m_lock_on = GetComponent<LockOn>();
		m_animator = GetComponent<Animator>();
	}
	
	void FixedUpdate ()
	{
		GameObject target = m_lock_on.Target;
		bool dodging = m_animator.GetBool(DODGING);
		Vector2 heading = target && !dodging ? (Vector2)(target.transform.position - transform.position) : m_movement.Heading;
		float x = heading.x;
		float y = heading.y;
		float speed = m_movement.Speed;
		if (speed > 0)
		{
			if (Mathf.Abs(x) > Mathf.Abs(y))
			{
				m_animator.SetTrigger(x > 0 ? RIGHT : LEFT);
			}
			else
			{
				m_animator.SetTrigger(y > 0 ? UP : DOWN);
			}
		}
		m_animator.SetInteger(SPEED, (int)speed);
	}
}
