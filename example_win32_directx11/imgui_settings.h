#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

inline ImColor main_color(0, 255, 255, 200);

inline ImColor background_color(15, 15, 15, 150);

inline ImColor second_color(28, 28, 28, 200);

inline ImColor text_color[2] = { ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 150) };

inline ImVec2 menu_size = ImVec2(700, 550);

inline float anim_speed = 8.f;

inline ImVec2 frame_size = ImVec2(300, 50);

inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline ImFont* second_font;
inline ImFont* bold_font;
inline ImFont* icon_font;

inline ImGuiWindow* main_window;
inline char search[64];

inline struct Notification {
    std::string message, information;
    ImColor color;
    ImRect bb;
    ImVec2 offset = ImVec2(400, 0);
    ImVec2 target_offset = ImVec2(400, 0);
    float timer = 30;
}notif_state;

class NotificationSystem {
public:

    inline void Notification(std::string message, std::string information, ImColor color)
    {
        //notif_state.offset.x = 0.f;

        notif_state.offset = ImVec2(400, 0);
        notif_state.target_offset = ImVec2(400, 0);
        static ImVec2 tab_offset(0.f, menu_size.x);
        notif_state.timer = 0;
        notif_state.color = color;
        notif_state.message = message;
        notif_state.information = information;
    }

    inline void DrawNotification()
    {
        notif_state.timer += ImGui::GetIO().DeltaTime * 10.f;
        notif_state.target_offset = notif_state.timer > 30.f ? ImVec2(400.f, 0.f) : ImVec2(0.f, 0.f);

        notif_state.offset = ImLerp(notif_state.offset, notif_state.target_offset, ImGui::GetIO().DeltaTime * 10.f);

        notif_state.bb = ImRect(ImGui::GetWindowPos() + ImGui::GetWindowSize() - ImVec2(360, 70) + ImVec2(notif_state.offset.x, 0.f), ImGui::GetWindowPos() + ImGui::GetWindowSize() - ImVec2(20, 20));

        ImGui::GetForegroundDrawList()->AddRectFilled(notif_state.bb.Min + ImVec2(notif_state.offset.x, 0.f), notif_state.bb.Max + ImVec2(notif_state.offset.x, 0.f), ImColor(23, 25, 30, 240), 2);

        ImGui::GetForegroundDrawList()->AddText(notif_state.bb.Min + ImVec2(12.5f, 6.f), main_color, notif_state.message.c_str());

        ImGui::PushFont(second_font);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(notif_state.bb.Min.x + 12.5f, notif_state.bb.Max.y - 9.5f - ImGui::CalcTextSize(notif_state.information.c_str()).y), text_color[1], notif_state.information.c_str());
        ImGui::PopFont();
    }
};

inline NotificationSystem notificationSystem;
