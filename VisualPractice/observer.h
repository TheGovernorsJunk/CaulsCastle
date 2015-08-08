#ifndef TE_OBSERVER_H
#define TE_OBSERVER_H

namespace te
{
	template <class EventType>
	class Observer
	{
	public:
        virtual ~Observer() {}
        virtual void onNotify(const EventType& evt) = 0;
	};
}

#endif
