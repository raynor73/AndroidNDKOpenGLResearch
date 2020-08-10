//
// Created by Igor Lapin on 10/08/2020.
//

#include "RigidBodyComponent.h"
#include "../../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/string"

const std::string RigidBodyComponent::TYPE_NAME = "RigidBodyComponent";

RigidBodyComponent::~RigidBodyComponent() {

}

void RigidBodyComponent::onGameObjectAttachedToParent() {
    GameObjectComponent::onGameObjectAttachedToParent();


}

void RigidBodyComponent::onGameObjectDetachedFromParent() {
    GameObjectComponent::onGameObjectDetachedFromParent();


}

std::shared_ptr <GameObjectComponent> RigidBodyComponent::clone() {
    return NULL;
}

