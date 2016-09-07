using UnityEngine;
using System.Collections;

public class LockOn : MonoBehaviour {

	public LayerMask LockMask = -1;
	public float LockDistance = 5f;
	public float TargetDistance = 10f;
	public string LockTag = "Enemy";

	public bool IsLocked
	{
		get
		{
			return m_latest_target != null;
		}
	}

	Movement m_movement;
	GameObject m_latest_target = null;
	float m_target_distance_sq;

	void Awake()
	{
		m_movement = GetComponent<Movement>();
		m_target_distance_sq = TargetDistance * TargetDistance;
	}

	public void Trigger(Vector2 direction)
	{
		if (!enabled) return;

		if (m_latest_target == null)
		{
			RaycastHit2D[] hits = Physics2D.RaycastAll(transform.position, direction, LockDistance, LockMask.value);
			foreach (RaycastHit2D hit in hits)
			{
				if (hit.collider.CompareTag(LockTag))
				{
					m_latest_target = hit.collider.gameObject;
					StartCoroutine(Lock(m_latest_target));
					break;
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
			Debug.Log("Locking: " + Time.time);
			yield return null;
		}
	}
}
