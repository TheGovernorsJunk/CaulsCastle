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
		spriteRenderer.flipX = direction == -1;
	}

	void OnIdle()
	{
		anim.SetBool("move", false);
	}

	void OnJump(bool isJumping)
	{
		anim.SetBool("jump", isJumping);
	}

	void OnGround()
	{
		if (anim.GetBool("jump"))
		{
			anim.SetBool("jump", false);
		}
	}

	void OnAttack(bool requestingAttack)
	{
		anim.SetBool("attack", requestingAttack);
	}

	void OnHit()
	{
		anim.SetBool("damage", true);
	}
}
