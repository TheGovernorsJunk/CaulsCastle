using UnityEngine;
using System.Collections;

public class DamageAnimation : MonoBehaviour
{
	Animator animator;
	int hitHash = Animator.StringToHash("hit");

	void Start()
	{
		animator = GetComponent<Animator>();
	}
	
	void OnTriggerEnter2D(Collider2D attacker)
	{
		if (attacker.tag == "HitBox")
		{
			animator.SetTrigger(hitHash);
		}
	}
}
