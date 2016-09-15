using UnityEngine;
using System.Collections;

public interface State
{
	void Update(GameObject gameObject);
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
		mCurrState.Update(gameObject);
	}
}
