#include "ui.h"



UI::UI()
{
}



UI::~UI()
{
}



void UI::RenderOverlayPanel(const char* pStr, glm::ivec2 vWindowSize)
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav;

    static bool bShowOverlayPanel = true;

    static int location = 0;
    const float fPAD = 40.0f;
    const ImGuiViewport* pViewport = ImGui::GetMainViewport();
    ImVec2 vWorkPos = pViewport->WorkPos;
    ImVec2 vWorkSize = pViewport->WorkSize;
    ImVec2 vWindowPos, vWindowPosPivot;
    vWindowPos.x = (location & 1) ? (vWorkPos.x + vWorkSize.x - fPAD) : (vWorkPos.x + fPAD);
    vWindowPos.y = (location & 2) ? (vWorkPos.y + vWorkSize.y - fPAD) : (vWorkPos.y + fPAD);
    vWindowPosPivot.x = (location & 1) ? 1.0f : 0.0f;
    vWindowPosPivot.y = (location & 2) ? 1.0f : 0.0f;

    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(vWindowPos, ImGuiCond_Always, vWindowPosPivot);
    ImGui::SetNextWindowSize(ImVec2(vWindowSize.x, vWindowSize.y));
    ImGui::SetNextWindowBgAlpha(0.25f);

    ImGui::Begin("Info Panel", &bShowOverlayPanel, windowFlags);
    ImGui::Text(pStr);
    ImGui::End();
}



void UI::RenderControlPanel(glm::ivec2 vWindowSize)
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav;

    static bool bShowControlPanel = true;

    static int location = 1;
    const float fPAD = 40.0f;
    const ImGuiViewport* pViewport = ImGui::GetMainViewport();
    ImVec2 vWorkPos = pViewport->WorkPos;
    ImVec2 vWorkSize = pViewport->WorkSize;
    ImVec2 vWindowPos, vWindowPosPivot;
    vWindowPos.x = (location & 1) ? (vWorkPos.x + vWorkSize.x - fPAD) : (vWorkPos.x + fPAD);
    vWindowPos.y = (location & 2) ? (vWorkPos.y + vWorkSize.y - fPAD) : (vWorkPos.y + fPAD);
    vWindowPosPivot.x = (location & 1) ? 1.0f : 0.0f;
    vWindowPosPivot.y = (location & 2) ? 1.0f : 0.0f;

    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(vWindowPos, ImGuiCond_Always, vWindowPosPivot);
    ImGui::SetNextWindowBgAlpha(0.25f);

    ImGui::Begin("Control Panel", &bShowControlPanel, windowFlags);
    if (ImGui::Button("Advance to next event"))
        bNewEvent = true;

    ImGui::End();
}
