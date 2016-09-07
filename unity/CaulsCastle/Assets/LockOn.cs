using UnityEngine;
using System.Collections;

public class LockOn : MonoBehaviour {

	public LayerMask LockMask = -1;
	public float LockDistance = 5f;
	public string LockTag = "Enemy";

	Movement m_movement;

	void Awake()
	{
		m_movement = GetComponent<Movement>();
	}

	public void Trigger(Vector2 direction)
	{
		RaycastHit2D[] hits = Physics2D.RaycastAll(transform.position, direction, LockDistance, LockMask.value);
		foreach (RaycastHit2D hit in hits)
		{
			if (hit.collider.CompareTag(LockTag))
			{
				Debug.Log("Lock!");
				break;
			}
		}
	}
}
