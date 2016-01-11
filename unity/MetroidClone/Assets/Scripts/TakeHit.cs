using UnityEngine;
using System.Collections;

public class TakeHit : MonoBehaviour
{
	void Start() {}

	void OnTriggerEnter2D(Collider2D attacker)
	{
		if (attacker.tag == "HitBox")
		{
			BroadcastMessage("OnHit");
		}
	}
}
