using UnityEngine;
using UnityEngine.Events;
using System.Collections;
using System.Linq;

public class LockOn : MonoBehaviour {

	public LayerMask LockMask = -1;
	public float LockDistance = 5f;
	public string LockTag = "Enemy";
	public float TargetDistance = 10f;

	public GameObject Target
	{
		get
		{
			return m_latest_target;
		}
	}
	public UnityEvent LockEvent
	{
		get
		{
			return m_lock_event;
		}
	}

	GameObject m_latest_target = null;
	float m_target_distance_sq;
	UnityEvent m_lock_event;

	void Awake()
	{
		m_target_distance_sq = TargetDistance * TargetDistance;
		m_lock_event = new UnityEvent();
	}

	public void Trigger(Vector2 direction)
	{
		if (!enabled) return;

		if (m_latest_target == null)
		{
			Vector2 position = transform.position;
			Collider2D[] collisions = Physics2D.OverlapCircleAll(position, LockDistance, LockMask);
			var matches = collisions.Where(c => c.CompareTag(LockTag));
			if (matches.Count() > 0)
			{
				Ray2D targetRay = new Ray2D(position, direction);
				matches = matches.Where(c => Vector2.Dot((Vector2)c.transform.position - position, targetRay.direction) >= 0);
				if (matches.Count() > 0)
				{
					m_latest_target = matches.Aggregate((currTarget, next) => Vector3.Cross(targetRay.direction, currTarget.transform.position - (Vector3)targetRay.origin).sqrMagnitude < Vector3.Cross(targetRay.direction, next.transform.position - (Vector3)targetRay.origin).sqrMagnitude ? currTarget : next).gameObject;
					StartCoroutine(Lock(m_latest_target));
					m_lock_event.Invoke();
				}
			}
		}
		else
		{
			m_latest_target = null;
		}
	}

	IEnumerator Lock(GameObject target)
	{
		while (target == m_latest_target)
		{
			if ((transform.position - target.transform.position).sqrMagnitude > m_target_distance_sq)
			{
				m_latest_target = null;
			}
			yield return null;
		}
	}
}
