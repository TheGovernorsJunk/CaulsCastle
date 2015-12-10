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
        Notifier(std::vector<std::shared_ptr<Observer<EventType>>>&& observers)
            : mObservers()
        {
            std::for_each(std::begin(observers), std::end(observers), [this](const std::shared_ptr<Observer<EventType>>& pObserver) {
                mObservers.push_back(std::weak_ptr<Observer<EventType>>{ pObserver });
            });
        }
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
            bool cleanupNeeded = false;
            std::for_each(std::begin(mObservers), std::end(mObservers), [&evt, &cleanupNeeded](const std::weak_ptr<Observer<EventType>>& wpObserver) {
                if (auto pObserver = wpObserver.lock()) {
                    pObserver->onNotify(evt);
                } else {
                    cleanupNeeded = true;
                }
            });

            // Remove obsolete pointers
            if (cleanupNeeded) {
                mObservers.erase(std::remove_if(mObservers.begin(), mObservers.end(), [this](const std::weak_ptr<Observer<EventType>>& wpObserver) {
                    return (wpObserver.lock() == std::shared_ptr<Observer<EventType>>());
                }));
            }
        }
    private:
        std::vector<std::weak_ptr<Observer<EventType>>> mObservers;
    };
}

#endif
