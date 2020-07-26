//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef GAME_GESTURES_DISPATCHER_H
#define GAME_GESTURES_DISPATCHER_H


#include <memory>
#include <set>
#include <unordered_map>
#include <game/touch_screen/GestureConsumerComponent.h>

static bool gestureConsumerComponentsComparator(
        const std::shared_ptr<GestureConsumerComponent>& lhs,
        const std::shared_ptr<GestureConsumerComponent>& rhs
) {
    return lhs->priority() > rhs->priority();
}

class GesturesDispatcher {

    std::set<std::shared_ptr<GestureConsumerComponent>, bool(*)(const std::shared_ptr<GestureConsumerComponent>&, const std::shared_ptr<GestureConsumerComponent>&)> m_gestureConsumers { &gestureConsumerComponentsComparator };
    std::unordered_map<int, std::shared_ptr<GestureConsumerComponent>> m_eventIdToGestureOwnerMap;
    std::unordered_map<std::shared_ptr<GestureConsumerComponent>, int> m_gestureOwnerToEventIdMap;

public:
    GesturesDispatcher() = default;
    GesturesDispatcher(const GesturesDispatcher&) = delete;
    GesturesDispatcher(GesturesDispatcher&&) = delete;

    void addGestureConsumer(std::shared_ptr<GestureConsumerComponent> gestureConsumer);
    void removeGestureConsumer(std::shared_ptr<GestureConsumerComponent> gestureConsumer);
    void removeAllGestureConsumers();

    void prepare();
    void onTouchEvent(const TouchEvent& touchEvent);

    GesturesDispatcher& operator=(const GesturesDispatcher&) = delete;
    GesturesDispatcher& operator=(GesturesDispatcher&&) = delete;

private:
    std::shared_ptr<GestureConsumerComponent> findMatchingGestureConsumer(const TouchEvent& touchEvent);
};


#endif //GAME_GESTURES_DISPATCHER_H
