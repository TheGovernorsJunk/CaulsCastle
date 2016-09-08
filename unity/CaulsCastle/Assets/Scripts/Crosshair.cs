using UnityEngine;
using System.Collections;

public class Crosshair : MonoBehaviour {

	public GameObject Avatar;
	public Vector2 Offset = new Vector2(0, 0);

	LockOn m_lock_on;
	SpriteRenderer m_sprite_renderer;

	void Awake()
	{
		m_lock_on = Avatar.GetComponent<LockOn>();
		m_sprite_renderer = GetComponent<SpriteRenderer>();
	}
	
	void Update ()
	{
		GameObject target = m_lock_on.Target;
		bool isLocked = target != null;
		m_sprite_renderer.enabled = isLocked;
		if (isLocked) transform.position = target.transform.position + (Vector3)Offset;
	}
}
