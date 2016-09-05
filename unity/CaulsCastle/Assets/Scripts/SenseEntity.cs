using UnityEngine;
using System.Collections;

public class SenseEntity : MonoBehaviour {

	void OnTriggerEnter2D(Collider2D collider)
	{
		print("Detected!");
	}
}
