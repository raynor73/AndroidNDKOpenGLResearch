//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef GAME_GESTURES_DISPATCHER_H
#define GAME_GESTURES_DISPATCHER_H


#include <memory>
#include <vector>
#include <unordered_map>
#include "GestureConsumerComponent.h"

class GesturesDispatcher {

    std::vector<std::shared_ptr<GestureConsumerComponent>> m_gestureConsumers;
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
    void updateOrder();
};


#endif //GAME_GESTURES_DISPATCHER_H
