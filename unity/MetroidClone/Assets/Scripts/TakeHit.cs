using UnityEngine;
using System.Collections;

public class TakeHit : MonoBehaviour
{
	Animator animator;
	int blockHash = Animator.StringToHash("Block");

	void Start() {}

	void Awake()
	{
		animator = GetComponent<Animator>();
	}

	void OnTriggerEnter2D(Collider2D attacker)
	{
		if (attacker.tag == "HitBox" && animator.GetCurrentAnimatorStateInfo(0).shortNameHash != blockHash)
		{
			Stats stats = attacker.GetComponentInParent<Stats>();
			DamageVector damageVector = new DamageVector()
			{ physicalDamage = stats.strength };
			BroadcastMessage("OnHit", damageVector);
		}
	}
}
