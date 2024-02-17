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

        void RenderOverlayPanel(std::string sStr, glm::ivec2 vWindowSize);
        void RenderControlPanel(glm::ivec2 vWindowSize);
        void RenderLevelEditPanel(glm::ivec2 vWindowSize);
        void RenderDebugPanel(std::vector<std::vector<std::string>*> vecDebugInfo);


    public:
        bool bNewEvent = false;

        bool bNewRoomNorth = false;
        bool bNewRoomSouth = false;
        bool bNewRoomEast  = false;
        bool bNewRoomWest  = false;


    private:
        bool m_bShowInfoPanel;
};
