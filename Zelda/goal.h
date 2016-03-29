#ifndef TE_GOAL_H
#define TE_GOAL_H

namespace te
{
	class Telegram;

	template<class T>
	class Goal
	{
	public:
		enum class Status
		{
			INACTIVE, ACTIVE, COMPLETED, FAILED
		};

		virtual void activate() = 0;
		virtual Status process() = 0;
		virtual void terminate() = 0;
		virtual bool handleMessage(const Telegram&) = 0;
		virtual void addSubgoal(const Goal&) {}
	};
}

#endif
