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
		grounded = true;
	}

	void OnCollisionExit2D()
	{
		grounded = false;
	}

}
