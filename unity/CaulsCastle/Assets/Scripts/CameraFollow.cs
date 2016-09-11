using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{
	public GameObject Subject;
	public float Distance = 1.0f;

	static int NormalX = Animator.StringToHash("NormalX");
	static int NormalY = Animator.StringToHash("NormalY");

	Animator m_subject_animator;
	LockOn m_subject_lock_on;

	void Awake ()
	{
		m_subject_animator = Subject.GetComponent<Animator>();
		m_subject_lock_on = Subject.GetComponent<LockOn>();
	}

	void LateUpdate ()
	{
		GameObject target = m_subject_lock_on.Target;
		if (target == null)
		{
			Vector2 position = Subject.transform.position;
			position = position + (new Vector2(m_subject_animator.GetFloat(NormalX), m_subject_animator.GetFloat(NormalY)) * Distance);
			transform.position = Vector3.Lerp(transform.position, new Vector3(position.x, position.y, transform.position.z), 0.1f);
		}
		else
		{
			Vector2 midway = Vector2.Lerp(target.transform.position, Subject.transform.position, 0.5f);
			transform.position = Vector3.Lerp(transform.position, new Vector3(midway.x, midway.y, transform.position.z), 0.1f);
		}
	}
}
