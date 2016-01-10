using UnityEngine;
using System.Collections;

public class GroundCheck : MonoBehaviour {

	void OnCollisionEnter2D()
	{
		transform.parent.BroadcastMessage("OnGround");
	}
}
