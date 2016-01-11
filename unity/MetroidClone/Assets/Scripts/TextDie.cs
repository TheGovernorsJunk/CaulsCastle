using UnityEngine;
using System.Collections;

public class TextDie : MonoBehaviour {

	public int frames = 50;
	int currFrame = 0;

	void Start ()
	{
		StartCoroutine("Die");
	}

	public void Reset()
	{
		currFrame = 0;
	}

	IEnumerator Die()
	{
		while (currFrame < frames)
		{
			++currFrame;
			yield return null;
		}
		Destroy(gameObject);
	}
}
