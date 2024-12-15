#include "main.h"


#include <tchar.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

static int dnd_counter = 0;

static bool checkbox[25];
static int slider_int = false;

const char* themes[]{ "Head","Chest", "Legs", "Hands" };

const char* auto_stop[]{ "Betwen shots", "Force accuracy", "Power accuracy", "Auto sleep" };

static int combo;

static int keybind;

static int iTabs;
static int menu_state;

float color_edit[4];

char login[64];
char password[64];

int main(int, char**)
{
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Example", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (menu_size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (menu_size.y / 2), menu_size.x, menu_size.y, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    SetWindowRgn(hwnd, CreateRoundRectRgn(0, 0, menu_size.x, menu_size.y, 10, 10), TRUE); // Создание скругленных углов


    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromMemoryTTF(&PoppinsRegular, sizeof PoppinsRegular, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());

    bold_font = io.Fonts->AddFontFromMemoryTTF(&PoppinsBold, sizeof PoppinsBold, 35, NULL, io.Fonts->GetGlyphRangesCyrillic());

    second_font = io.Fonts->AddFontFromMemoryTTF(&PoppinsRegular, sizeof PoppinsRegular, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());

    icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);

    ImGuiStyle& s = ImGui::GetStyle();

    s.FramePadding = ImVec2(5, 3);
    s.WindowPadding = ImVec2(0, 0);
    s.FrameRounding = 1.f;
    s.WindowRounding = 2.f;
    s.WindowBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildBorderSize = 10;
    s.WindowShadowSize = 0;

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            LoadImages();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(700, 550));
            ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                main_window = ImGui::GetCurrentWindow();
                auto draw = ImGui::GetWindowDrawList();
                const auto& p = ImGui::GetWindowPos();

                draw->AddRectFilled(p, p + menu_size, ImColor(0.03f, 0.03f, 0.03f, 0.4f), 2.f);

                draw->AddRectFilled(p, p + ImVec2(755, 50), background_color, 2.f, ImDrawFlags_RoundCornersTop);
                draw->AddRectFilled(p, p + ImVec2(70, 550), background_color, 2.f, ImDrawFlags_RoundCornersLeft);
                draw->AddRectFilled(p + ImVec2(0, 50), p + ImVec2(755, 52), second_color);
                draw->AddRectFilled(p + ImVec2(70, 0), p + ImVec2(72, 550), second_color);

                draw->AddImageRounded(avatar, p + ImVec2(80, 5), p + ImVec2(120, 45), ImVec2(0, 0), ImVec2(1, 1), ImColor(1.f, 1.f, 1.f, 1.f), 360);

                if (menu_state == 1) {
                    draw->AddText(p + ImVec2(130, 5), ImColor(1.f, 1.f, 1.f, 1.f), "Fujiwara");
                   
                                       
                    ImGui::SetCursorPos(ImVec2(10, 60));
                    ImGui::BeginChild("Tabs", ImVec2(60, 490), false, ImGuiWindowFlags_NoScrollbar);

                    ImGui::Tab("d", &iTabs, 0);
                    ImGui::Tab("c", &iTabs, 1);
                    ImGui::Tab("s", &iTabs, 2);
                    ImGui::Tab("g", &iTabs, 3);
                    ImGui::Tab("2", &iTabs, 4);
                    ImGui::EndChild();

                    if (iTabs == 0) {
                        ImGui::SetCursorPos(ImVec2(80, 60));
                        ImGui::BeginChild("General", ImVec2(300, 490), false, ImGuiWindowFlags_NoScrollbar);
                        ImGui::Checkbox("Aimbot", &checkbox[0], "Enable aimbot", &keybind);
                        ImGui::SliderInt("Aim fov", &slider_int, 0, 100);
                        ImGui::Combo("Hitbox selection", &combo, auto_stop, IM_ARRAYSIZE(themes), 4, "Select hitbox");
                        ImGui::ColorEdit4("Hitmarker color", (float*)color_edit, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs, "Hit marker color");
                        ImGui::Checkbox("Doubletap", &checkbox[1], "Exploit doubletap");
                        ImGui::Checkbox("Hide shots", &checkbox[2]);
                        if (ImGui::Checkbox("Auto peek", &checkbox[3]))
                            notificationSystem.Notification("Hello world", "Applied!", main_color);
                        ImGui::Checkbox("Fake lags", &checkbox[4], "Enable ping to 350");
                        ImGui::Checkbox("Jitter move", &checkbox[5]);
                        ImGui::Checkbox("Overwatch revealer", &checkbox[6]);
                        ImGui::Checkbox("OBS bypass", &checkbox[7]);
                        ImGui::Checkbox("Fast stop", &checkbox[8]);
                        ImGui::Checkbox("Fast duck", &checkbox[9]);
                        ImGui::Checkbox("Bunnyhop", &checkbox[10]);
                        ImGui::EndChild();

                        ImGui::SetCursorPos(ImVec2(390, 60));
                        ImGui::BeginChild("Checkboxes", ImVec2(300, 490), false, ImGuiWindowFlags_NoScrollbar);
                        static bool checkboxes[50];
                        for (int i = 0; i < 50; i++) {
                            ImGui::Checkbox(std::to_string(i).c_str(), &checkboxes[i], "Test checkbox");
                        }
                        ImGui::EndChild();
                    }
                    if (iTabs == 1) {
                    }
                }
                else
                {
                    ImGui::SetCursorPos(ImVec2(105, 195));
                    ImGui::BeginChild("Inputs", ImVec2(500, 230), false, ImGuiWindowFlags_NoScrollbar); {
                        ImGui::PushFont(bold_font);
                        draw->AddText(center_text(p + ImVec2(0, 100), p + ImVec2(700, 195), "WELCOME"), ImColor(1.f, 1.f, 1.f, 1.f), "WELCOME");
                        ImGui::PopFont();
                        ImGui::InputTextEx("Login", NULL, login, 64, ImVec2(490, 50), 0);
                        ImGui::InputTextEx("Password", NULL, password, 64, ImVec2(490, 50), 0);
                        if (ImGui::Button("Autorize", ImVec2(490, 50)))
                            menu_state = 1;

                    }ImGui::EndChild();
                }
                Trinage_background();
                notificationSystem.DrawNotification();
            }
            move_window();
            ImGui::End();
            RenderBlur(hwnd);
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
