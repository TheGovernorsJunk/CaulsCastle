using UnityEngine;
using System.Collections;

public abstract class State
{
	public abstract State Update(GameObject gameObject);
	public virtual void Enter() {}
	public virtual void Exit() {}
}

public class StateMachine
{
	State mCurrState;

	public StateMachine(State initialState)
	{
		mCurrState = initialState;
	}

	public void Update(GameObject gameObject)
	{
		State newState = mCurrState.Update(gameObject);
		if (newState != null)
		{
			mCurrState.Exit();
			mCurrState = newState;
			mCurrState.Enter();
		}
	}
}
