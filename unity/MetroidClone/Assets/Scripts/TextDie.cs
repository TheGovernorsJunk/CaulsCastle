using UnityEngine;
using System.Collections;

public class TextDie : MonoBehaviour {

	int frames = 40;

	void Start ()
	{
		StartCoroutine("Die");
	}

	IEnumerator Die()
	{
		int currFrame = 0;
		while (currFrame < frames)
		{
			++currFrame;
			yield return null;
		}
		Destroy(gameObject);
	}
}
