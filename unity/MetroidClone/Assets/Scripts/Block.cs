using UnityEngine;
using System.Collections;

public class Block : MonoBehaviour {

	// Update is called once per frame
	void Update ()
	{
		int block = (int)Input.GetAxisRaw("Fire3");
		BroadcastMessage("OnBlock", block == 1);
	}
}
