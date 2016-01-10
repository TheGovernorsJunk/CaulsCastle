using UnityEngine;
using System.Collections;

public class ScrollLoop : MonoBehaviour
{
	public int moveUnits = 1;

	void OnTriggerEnter2D(Collider2D other)
	{
		SpriteRenderer renderer = other.GetComponent<SpriteRenderer>();
		other.transform.Translate(new Vector3(
			moveUnits * renderer.bounds.size.x, 0, 0));
	}
}
