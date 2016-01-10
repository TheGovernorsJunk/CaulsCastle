using UnityEngine;
using System.Collections;

public class CameraScript : MonoBehaviour {

	public GameObject subject;
	
	// Update is called once per frame
	void Update ()
	{
		Vector3 subPosition = subject.transform.position;
		subPosition.z = transform.position.z;
		transform.position = subPosition;
	}
}
