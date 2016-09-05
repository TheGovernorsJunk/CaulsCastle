using UnityEngine;
using System.Collections;

public class SenseEntity : MonoBehaviour {

	public string SenseTag = "Player";

	void OnTriggerEnter2D(Collider2D collider)
	{
		if (collider.CompareTag(SenseTag))
		{
			print("Player detected");
		}
	}
}
