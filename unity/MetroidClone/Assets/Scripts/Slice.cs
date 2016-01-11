using UnityEngine;
using System.Collections;

public class Slice : MonoBehaviour {

	void OnTriggerEnter2D(Collider2D target)
	{
		Debug.Log("Hit!");
	}

	void OnMove(int direction)
	{
		if (direction == 1)
		{
			transform.localScale = new Vector3(1,1,1);
		}
		else if (direction == -1)
		{
			transform.localScale = new Vector3(-1,1,1);
		}
	}
}
