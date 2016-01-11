using UnityEngine;
using System.Collections;

public class TakeHit : MonoBehaviour
{
	void Start() {}

	void OnTriggerEnter2D(Collider2D attacker)
	{
		if (attacker.tag == "HitBox")
		{
			Stats stats = attacker.GetComponentInParent<Stats>();
			DamageVector damageVector = new DamageVector()
			{ physicalDamage = stats.strength };
			BroadcastMessage("OnHit", damageVector);
		}
	}
}
