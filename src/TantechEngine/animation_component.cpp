#include "animation_component.h"
#include "mesh_manager.h"

namespace te
{
    AnimationComponent::AnimationComponent(size_t capacity)
        : Component(capacity) {}

    static void checkExistingKey(const std::map<int, std::shared_ptr<const Animation>> animations, int key)
    {
        auto it = animations.find(key);
        if (it == animations.end()) {
            throw std::runtime_error{ "AnimationComponent::setAnimations: no animation for given initial key." };
        }
    }

    static void checkErrors(const std::map<int, std::shared_ptr<const Animation>> animations, int initialKey)
    {
        if (animations.size() == 0) {
            throw std::runtime_error{ "AnimationComponent::setAnimations: must have at least one animation." };
        }
        for (auto it = animations.begin(); it != animations.end(); ++it) {
            if (it->second->frames.size() == 0) {
                throw std::runtime_error{ "AnimationComponent::setAnimations: must have at least one frame." };
            }
        }

        checkExistingKey(animations, initialKey);
    }

    void AnimationComponent::setAnimations(const Entity& entity, const std::map<int, std::shared_ptr<const Animation>>& animations, int initialKey)
    {
        checkErrors(animations, initialKey);

        // Create instance
        if (!hasInstance(entity)) {
            createInstance(entity, { animations, nullptr, 0, 0 });
        } else {
            at(entity) = { animations, nullptr, 0, 0 };
        }

        // Correct pointer
        AnimationInstance& instance = at(entity);
        instance.currAnimation = instance.animations.find(initialKey)->second.get();
    }

    void AnimationComponent::setAnimations(const Entity& entity, std::map<int, std::shared_ptr<const Animation>>&& animations, int initialKey)
    {
        checkErrors(animations, initialKey);

        // Create instance
        if (!hasInstance(entity)) {
            createInstance(entity, { std::move(animations), nullptr, 0, 0 });
        } else {
            at(entity) = { std::move(animations), nullptr, 0, 0 };
        }

        // Correct pointer
        AnimationInstance& instance = at(entity);
        instance.currAnimation = instance.animations.find(initialKey)->second.get();
    }

    void AnimationComponent::setAnimation(const Entity& entity, int key)
    {
        AnimationInstance* pInstance = nullptr;
        if (hasInstance(entity)) {
            pInstance = &at(entity);
        } else {
            throw std::runtime_error{ "AnimationComponent::setAnimation: entity has no animations." };
        }

        checkExistingKey(pInstance->animations, key);
        pInstance->currAnimation = pInstance->animations.find(key)->second.get();
        pInstance->currDuration = 0;
        pInstance->currFrameIndex = 0;
    }
}
