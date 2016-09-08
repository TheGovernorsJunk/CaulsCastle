using UnityEngine;
using System.Collections;

public class PlayerInput : MonoBehaviour {

	public GameObject Avatar;
	public float MovementThreshold = 0.3f;
	public float LockInfluenceThreshold = 0.3f;

	Movement m_movement;
	LightAttack m_light_attack;
	LockOn m_lock_on;

	void Awake()
	{
		m_movement = Avatar.GetComponent<Movement>();
		m_light_attack = Avatar.GetComponent<LightAttack>();
		m_lock_on = Avatar.GetComponent<LockOn>();
	}

	bool attacking = false;
	bool locking = false;
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

		float lockEvent = Input.GetAxisRaw("Lock");
		if (lockEvent > 0 && !locking)
		{
			float lockX = Input.GetAxisRaw("U");
			float lockY = Input.GetAxisRaw("V");
			Vector2 lockDirection = Mathf.Abs(lockX) > LockInfluenceThreshold || Mathf.Abs(lockY) > LockInfluenceThreshold
				? new Vector2(lockX, lockY)
				: new Vector2(m_movement.Heading.x, m_movement.Heading.y);
			m_lock_on.Trigger(lockDirection);
			locking = true;
		}
		else if (lockEvent == 0)
		{
			locking = false;
		}
	}
}
