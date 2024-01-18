#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <imgui.h>

#include <iostream>



class UI
{
    public:
        UI();
        ~UI();

        void RenderOverlayPanel(const char* pStr, glm::ivec2 vWindowSize);
        void RenderControlPanel(glm::ivec2 vWindowSize);
        void RenderDebugPanel(std::vector<std::vector<std::string>*> vecDebugInfo);


    public:
        bool bNewEvent = false;


    private:
        bool m_bShowInfoPanel;
};
