using UnityEngine;
using System.Collections;

public class LightAttack : MonoBehaviour {

	static int ATTACK = Animator.StringToHash("attack");

	Animator m_animator;

	void Awake()
	{
		m_animator = GetComponent<Animator>();
	}

	public void Trigger()
	{
		if (enabled) m_animator.SetTrigger(ATTACK);
	}
}
