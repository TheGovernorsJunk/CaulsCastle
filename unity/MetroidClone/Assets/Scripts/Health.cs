using UnityEngine;
using System.Collections;

public class Health : MonoBehaviour
{
	public int HP
	{
		get { return hp; }
	}

	private Stats stats;
	private int maxHP;
	private int hp;

	void Start ()
	{
		stats = GetComponent<Stats>();
		maxHP = hp = stats.vitality * 10;
	}
	
	void OnHit(DamageVector damageVector)
	{
		int damage = (int)(damageVector.physicalDamage - (stats.resistance * 0.2));
		Debug.Log(string.Format("Took {0} damage!", damage));
		hp -= damage;
		if (hp <= 0)
			Destroy(gameObject);
	}
}
