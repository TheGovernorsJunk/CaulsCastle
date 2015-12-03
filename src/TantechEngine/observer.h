#ifndef TE_OBSERVER_H
#define TE_OBSERVER_H

#include <vector>
#include <memory>
#include <cassert>

namespace te
{
	template <class EventType>
	class Observer
	{
	public:
        virtual ~Observer() {}
        virtual void onNotify(const EventType& evt) = 0;
	};

    template <class EventType>
    class Notifier
    {
    public:
        virtual ~Notifier() {}
        void addObserver(std::shared_ptr<Observer<EventType>> newObserver)
        {
            assert(newObserver);
            if (std::find(mObservers.begin(), mObservers.end(), newObserver) == mObservers.end()) {
                mObservers.push_back(newObserver);
            }
        }
    protected:
        void notify(const EventType& evt)
        {
            std::for_each(std::begin(mObservers), std::end(mObservers), [&evt](std::shared_ptr<Observer<EventType>>& pObserver) {
                pObserver->onNotify(evt);
            });
        }
    private:
        std::vector<std::shared_ptr<Observer<EventType>>> mObservers;
    };
}

#endif
