using UnityEngine;
using System.Collections;

public class RenderSort : MonoBehaviour
{
	SpriteRenderer mRenderer;

	void Awake ()
	{
		mRenderer = GetComponent<SpriteRenderer>();
	}
	
	void LateUpdate ()
	{
		mRenderer.sortingOrder = (int)Camera.main.WorldToScreenPoint(mRenderer.bounds.min).y * -1;
	}
}
