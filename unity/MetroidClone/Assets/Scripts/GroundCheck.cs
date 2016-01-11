using UnityEngine;
using System.Collections;

public class GroundCheck : MonoBehaviour {

	public bool Grounded
	{
		get
		{
			return grounded;
		}
	}

	private bool grounded;

	void OnCollisionEnter2D()
	{
		transform.parent.BroadcastMessage("OnGround");
		grounded = true;
	}

	void OnCollisionExit2D()
	{
		grounded = false;
	}

}
