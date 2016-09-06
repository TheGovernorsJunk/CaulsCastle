using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{

	public GameObject Subject;
	public float Distance = 1.0f;

	Movement m_subject_movement;

	void Awake ()
	{
		m_subject_movement = Subject.GetComponent<Movement>();
	}

	void Update ()
	{
		Vector2 position = Subject.transform.position;
		position = position + (m_subject_movement.Heading * Distance);
		transform.position = Vector3.Lerp(transform.position, new Vector3(position.x, position.y, transform.position.z), 0.1f);
	}
}
