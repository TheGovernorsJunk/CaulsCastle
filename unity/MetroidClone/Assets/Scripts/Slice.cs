using UnityEngine;
using System.Collections;

public class Slice : MonoBehaviour
{
	void OnMove(int direction)
	{
		if (direction == 1)
		{
			Vector3 scale = transform.localScale;
			scale.x = Mathf.Abs(transform.localScale.x);
			transform.localScale = scale;
		}
		else if (direction == -1)
		{
			Vector3 scale = transform.localScale;
			scale.x = -Mathf.Abs(transform.localScale.x);
			transform.localScale = scale;
		}
	}
}
