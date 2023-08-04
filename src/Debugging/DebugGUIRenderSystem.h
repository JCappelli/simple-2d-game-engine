#ifndef DEBUGGUIRENDERSYSTEM_H
#define DEBUGGUIRENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Events/InputEvent.h"
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
    void Update();
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

void DebugGUIRenderSystem::Update()
{
    if (isEnabled)
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    }
}

#endif