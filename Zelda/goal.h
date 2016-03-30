#ifndef TE_GOAL_H
#define TE_GOAL_H

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;

	template<class T>
	class Goal
	{
	public:
		enum class Status
		{
			INACTIVE, ACTIVE, COMPLETED, FAILED
		};

		Goal()
			: mStatus(Status::INACTIVE)
		{}

		virtual void activate() = 0;
		virtual Status process(const sf::Time& dt) = 0;
		virtual void terminate() = 0;
		virtual bool handleMessage(const Telegram&)
		{
			return false;
		}
		virtual void addSubgoal(const Goal&) {}

		bool isActive() const
		{
			return mStatus == Status::ACTIVE;
		}
		bool isInactive() const
		{
			return mStatus == Status::INACTIVE;
		}
		bool isCompleted() const
		{
			return mStatus == Status::COMPLETED;
		}
		bool hasFailed() const
		{
			return mStatus == Status::FAILED;
		}

	protected:
		void setStatus(Status status)
		{
			mStatus = status;
		}
	private:
		Status mStatus;
	};
}

#endif
