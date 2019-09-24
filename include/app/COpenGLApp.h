#pragma once

#include <LScene.h>

#include <camera/CICamera.h>
#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>

#include <LMasterRenderer.h>

#include <assets/CTextureManager.h>
#include <shaders/LShaderManager.h>

#include <input/CInputHandler.h>

#include <assets/CTextureManager.h>
#include <app/COpenGLWindow.h>

#include <utils/CLogger.h>
#include <utils/CTime.h>
#include <utils/CImguiUi.h>
#include <utils/CDebugDrawer.h>

namespace engine
{

    class COpenGLApp
    {
    public :
        static COpenGLApp* GetInstance();
        static COpenGLWindow* GetWindow();

        COpenGLApp();
        virtual ~COpenGLApp();

        void init();

        void begin();

        void update();

        void end();

        LScene* scene();

        bool isActive();

        float frametime();

        float fps();

        LMasterRenderer* renderer() const { return m_masterRenderer; }

    protected :

        virtual void _initUser();
        virtual void _updateUser();

    protected :

        static COpenGLApp* s_instance;

        LMasterRenderer* m_masterRenderer;
        LScene* m_scenePtr;

        COpenGLWindow* m_windowPtr;
        CImguiUi* m_uiPtr;

        float m_timeNow;
        float m_timeDelta;
    };

}