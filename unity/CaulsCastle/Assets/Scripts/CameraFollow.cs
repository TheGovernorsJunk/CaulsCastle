using UnityEngine;
using System.Collections;

public class CameraFollow : MonoBehaviour
{
	public GameObject Subject;
	public float Distance = 1.0f;

	MobMotion m_subject_motion;
	LockOn m_subject_lock_on;

	void Awake ()
	{
		m_subject_motion = Subject.GetComponent<MobMotion>();
		m_subject_lock_on = Subject.GetComponent<LockOn>();
	}

	void LateUpdate ()
	{
		GameObject target = m_subject_lock_on.Target;
		if (target == null)
		{
			Vector2 position = Subject.transform.position;
			position = position + (m_subject_motion.Heading * Distance);
			transform.position = Vector3.Lerp(transform.position, new Vector3(position.x, position.y, transform.position.z), 0.1f);
		}
		else
		{
			Vector2 midway = Vector2.Lerp(target.transform.position, Subject.transform.position, 0.5f);
			transform.position = Vector3.Lerp(transform.position, new Vector3(midway.x, midway.y, transform.position.z), 0.1f);
		}
	}
}
