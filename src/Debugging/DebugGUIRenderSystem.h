#ifndef DEBUGGUIRENDERSYSTEM_H
#define DEBUGGUIRENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
#include "../Game/GameObjectLoader.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

class DebugGUIRenderSystem : public System
{
private:
    bool isEnabled = false;
    void OnButtonPressed(InputButtonEvent& event);
public:
    DebugGUIRenderSystem() = default;
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);
    void Update(const std::unique_ptr<Registry>& registry);
};

void DebugGUIRenderSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
    eventBus->SubscribeToEvent<InputButtonEvent>(this, &DebugGUIRenderSystem::OnButtonPressed);
}

void DebugGUIRenderSystem::OnButtonPressed(InputButtonEvent& event)
{
    if (event.actionType == INPUT_BUTTON_DEBUG_DRAW_PRESS)
    {
        isEnabled = !isEnabled;
    }
}

void DebugGUIRenderSystem::Update(const std::unique_ptr<Registry>& registry)
{
    if (isEnabled)
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
        
        if (ImGui::Begin("Enemy Spawner"))
        {
            static int x = 10;
            static int y = 10;

            ImGui::InputInt("Tile X", &x);
            ImGui::InputInt("Tile Y", &y);

            if (ImGui::Button("Spawn"))
            {
                GameObjectLoader::LoadEnemyEntity(x * 16, y * 16, registry);
            }
        }
        ImGui::End();

        ImGui::Render();
        
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    }
}

#endif