using UnityEngine;
using System.Collections;

public class CameraScript : MonoBehaviour {

	public GameObject subject;
	public Vector2 offset = new Vector2(0, 0);
	
	// Update is called once per frame
	void Update ()
	{
		Vector3 subPosition = new Vector3(
			subject.transform.position.x + offset.x,
			subject.transform.position.y + offset.y,
			transform.position.z);
		transform.position = subPosition;
	}
}
