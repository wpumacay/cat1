#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine
{

    class CImGuiManager
    {

    public :

        CImGuiManager( GLFWwindow* glfwWindowPtr );
        ~CImGuiManager();

        void begin();
        void render();

        void setActive( bool active ) { m_active = active; }
        void setDockingSpace( bool enabled ) { m_useDockingSpace = enabled; }
        void setDockingSpacePassthrough( bool enabled );
        bool active() const { return m_active; }
        bool usesDocking() const { return m_useDockingSpace; }
        bool usesDockingPassthrough() const;

    private :

        void _configureDockingSpace();

    private :

        bool m_active;
        bool m_useDockingSpace;

        ImGuiDockNodeFlags  m_dockSpaceFlags;       // docking configuration options
        ImGuiWindowFlags    m_dockSpaceWindowFlags; // central-window (docking node) configuration

        /* handle to glfw window to copy (or create more, if viewports enabled???) contexts */
        GLFWwindow* m_glfwWindowPtr;
    };

}