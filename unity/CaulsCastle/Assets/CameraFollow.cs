using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{

	public GameObject Subject;

	void Update ()
	{
		Vector2 position = Subject.transform.position;
		transform.position = new Vector3(position.x, position.y, transform.position.z);
	}
}
