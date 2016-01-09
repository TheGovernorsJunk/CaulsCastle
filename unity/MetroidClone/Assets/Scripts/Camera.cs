using UnityEngine;
using System.Collections;

public class Camera : MonoBehaviour {

	public GameObject subject;
	
	// Update is called once per frame
	void Update ()
	{
		Vector3 subPosition = subject.transform.position;
		subPosition.z = transform.position.z;
		transform.position = subPosition;
	}
}
