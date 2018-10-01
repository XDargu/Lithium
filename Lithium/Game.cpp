#include "stdafx.h"

#include "Game.h"
#include "Core/Graphics/Shaders/Shader.h"
#include <vector>
#include "Core\Memory\FreeListAllocator.h"
#include "Core\Memory\MemoryManager.h"
#include "Core\Globals\DebugGlobals.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"

static Game gGame;

PixelShader     lPixelShader;
VertexShader    lVertexShader;

cEgMesh mTestMesh;

class Vec3f
{
public:
    float	x, y, z;

    Vec3f(float vx, float vy, float vz)
    {
        x = vx;
        y = vy;
        z = vz;
    }

    float GetLength()
    {
        return sqrt(x*x + y*y + z*z);
    }

    static float Dot(const Vec3f& v1, const Vec3f& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    void Normalize()
    {
        float length = GetLength();

        length = 1.0f / length;

        x *= length;
        y *= length;
        z *= length;
    }

    Vec3f &Vec3f::operator*=(float value)
    {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3f operator+(const Vec3f &v)
    {
        return Vec3f(x + v.x, y + v.y, z + v.z);
    }

    Vec3f operator-(const Vec3f &v)
    {
        return Vec3f(x - v.x, y - v.y, z - v.z);
    }
};

Game& Game::GetInstance()
{
    return gGame;
}


sBool
Game::Construct()
{
    sBool lbSuccess = mRenderManager.Construct();

    ID3DBlob* lpVSBlob = NULL;

    lbSuccess &= lPixelShader.Compile("Tutorial07.fx", "PS");
    lbSuccess &= lVertexShader.Compile("Tutorial07.fx", "VS", gVertexLayoutPositionColor);
    
    render_constants::Construct();
    
    render_utils::CreateCube(mTestMesh, 1);
    render_utils::CreateGrid(gDebugGlobals.mGridMesh, 10);

    return lbSuccess;
}

void
Game::Update()
{
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    /*{
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }*/

    
}

void
Game::Render()
{
    // Time
    static sFloat32 t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    // Clear the back buffer 
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    mRenderManager.ctx->ClearRenderTargetView(mRenderManager.render_target_view, ClearColor);
    mRenderManager.ctx->ClearDepthStencilView(mRenderManager.depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Initialize the view matrix
    Vector3 lEye = Vector3(5.0f * sin(t*0.5f), 2.0f + sin(t), 5.0f * cos(t*0.5f));
    Vector3 lAt = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 lUp = Vector3(0.0f, 1.0f, 0.0f);

    Matrix lView = Matrix::CreateLookAt(lEye, lAt, lUp);

    // Initialize the projection matrix
    Matrix lProjection = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV2, mRenderManager.xres / (FLOAT)mRenderManager.yres, 0.01f, 100.0f);

    render_constants::ActivateConstantBuffer(0);

    render_constants::SetViewProjection(lView, lProjection);

    // Initialize the world matrix
    Matrix lWorld1 = Matrix::Identity;

    lWorld1 = XMMatrixRotationY(-t);
    
    render_constants::SetWorldMatrix(lWorld1);

    //
    // Render the first cube
    //
    lVertexShader.Activate();
    lPixelShader.Activate();

    cEgMesh::Activate(mTestMesh);
    cEgMesh::Render(mTestMesh);

    // 
    lWorld1 = Matrix::Identity;
    render_constants::SetWorldMatrix(lWorld1);

    //
    // Render the grid
    //
    cEgMesh::Activate(gDebugGlobals.mGridMesh);
    cEgMesh::Render(gDebugGlobals.mGridMesh);

    static bool show_another_window = false;
    static ImVec4 clear_col = ImColor(114, 144, 154);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&clear_col); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    {
        ImGui::Begin("Framerate", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowSize(ImVec2(240, 20), ImGuiSetCond_FirstUseEver);
        ImGui::SetWindowPos(ImVec2(4, 2), ImGuiSetCond_FirstUseEver);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        //ImGui::ShowTestWindow();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Present the information rendered to the back buffer to the front buffer (the screen)
    mRenderManager.swap_chain->Present(1, 0);
}

sBool
Game::ShouldFinishGame()
{
    return FALSE;
}

void
Game::Release()
{
    mRenderManager.Release();
    render_constants::Release();
}