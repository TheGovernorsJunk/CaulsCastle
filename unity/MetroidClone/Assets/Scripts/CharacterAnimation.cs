using UnityEngine;
using System.Collections;

public class CharacterAnimation : MonoBehaviour {

	SpriteRenderer spriteRenderer;
	Animator anim;

	void Start()
	{
		spriteRenderer = GetComponent<SpriteRenderer>();
		anim = GetComponent<Animator>();
	}

	void OnMove(int direction)
	{
		anim.SetBool("move", true);
		anim.SetBool("idle", false);
		spriteRenderer.flipX = direction == -1;
	}

	void OnIdle()
	{
		anim.SetBool("idle", true);
		anim.SetBool("move", false);
	}
}
