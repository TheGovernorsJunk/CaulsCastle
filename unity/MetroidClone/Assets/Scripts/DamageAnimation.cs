using UnityEngine;
using System.Collections;

public class DamageAnimation : MonoBehaviour
{
	Animator animator;
	int hitHash = Animator.StringToHash("hit");
	int blockHash = Animator.StringToHash("Block");

	void Start()
	{
		animator = GetComponent<Animator>();
	}
	
	void OnTriggerEnter2D(Collider2D attacker)
	{
		if (attacker.tag == "HitBox" && animator.GetCurrentAnimatorStateInfo(0).shortNameHash != blockHash)
		{
			animator.SetTrigger(hitHash);
		}
	}
}
