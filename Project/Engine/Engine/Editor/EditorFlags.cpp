#include "pch.h"
#include "EditorFlags.h"

FLOAT WindowFlags::ASPECT_RATIO = 9.f / 16.f;

ImGuiWindowFlags WindowFlags::EXPLORER_FLAGS = 0;

ImGuiWindowFlags WindowFlags::RENDER_WINDOW_FLAGS =
ImGuiWindowFlags_NoScrollbar |
ImGuiWindowFlags_NoNav |
ImGuiWindowFlags_NoScrollWithMouse |
ImGuiWindowFlags_NoBringToFrontOnFocus;