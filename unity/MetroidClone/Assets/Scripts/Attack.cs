using UnityEngine;
using System.Collections;

public class Attack : MonoBehaviour
{
	void FixedUpdate()
	{
		int slice = (int)Input.GetAxisRaw("Fire1");
		BroadcastMessage("OnAttack", slice == 1);
	}
}
