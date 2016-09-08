using UnityEngine;
using System.Collections;

public class Crosshair : MonoBehaviour {

	public GameObject Avatar;
	public Vector2 Offset = Vector2.zero;

	LockOn m_lock_on;
	SpriteRenderer m_sprite_renderer;

	void Awake()
	{
		m_lock_on = Avatar.GetComponent<LockOn>();
		m_sprite_renderer = GetComponent<SpriteRenderer>();
	}

	void Start()
	{
		m_lock_on.LockEvent.AddListener(Reveal);
	}

	void Reveal()
	{
		if (!m_sprite_renderer.enabled)
		{
			m_sprite_renderer.enabled = true;
			transform.parent = m_lock_on.Target.transform;
			transform.localPosition = Offset;
			StartCoroutine(Continue());
		}
	}

	IEnumerator Continue()
	{
		GameObject target = m_lock_on.Target;
		while (target)
		{
			transform.parent = m_lock_on.Target.transform;
			transform.localPosition = Offset;
			yield return null;
			target = m_lock_on.Target;
		}
		m_sprite_renderer.enabled = false;
		transform.parent = null;
	}
}
