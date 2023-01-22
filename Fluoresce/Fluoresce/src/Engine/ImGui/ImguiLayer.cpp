//==============================================================================//
// Name : ImguiLayer.cpp														// 
// Describe : 	Imguiレイヤー													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/ImGui/ImguiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Engine/Core/Application.h"
#include "Engine/ImGui/ImguiSerializer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fluoresce {

	static FrVec4 ConvertFrVec4(const ImVec4& vec)
	{
		return FrVec4{ vec.x, vec.y, vec.z, vec.w };
	}

	static ImVec4 ConvertImVec4(const FrVec4& vec)
	{
		return ImVec4{ vec.x, vec.y, vec.z, vec.w };
	}

	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer")
	{

	}

	ImguiLayer::~ImguiLayer()
	{

	}

	void ImguiLayer::OnAttach()
	{
		constexpr float32 fontSize = 18.0f;

		// Dear Imgui初期化(ドッキング使用)
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
		// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
		//io.IniFilename = "imgui.ini";

		io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans/OpenSans-Regular.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("resources/fonts/opensans/OpenSans-Bold.ttf", fontSize);

		// カスタマイズ設定初期化
		SetDefaultFont();
		SetDefaultItemColor();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		// Imgui GL3初期化
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImguiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImguiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImguiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImguiLayer::DockspaceBegin()
	{
		// ImguiDemo.cpp からのコピー

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	void ImguiLayer::DockspaceEnd()
	{
		ImGui::End();
	}

	void ImguiLayer::SetDefaultFont()
	{
		m_FontIndex = 0;
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->Fonts[0];
		io.FontDefault = font;
	}

	void ImguiLayer::SetFont(sint32 index)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (m_FontIndex < io.Fonts->Fonts.Size)
		{
			ImFont* font = io.Fonts->Fonts[m_FontIndex];
			io.FontDefault = font;
		}
	}

	void ImguiLayer::SetDefaultItemColor()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.225f, 0.23f, 0.235f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3625f, 0.3675f, 0.3725f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.225f, 0.23f, 0.235f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3625f, 0.3675f, 0.3725f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.225f, 0.23f, 0.235f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3625f, 0.3675f, 0.3725f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.3925f, 0.3930f, 0.3935f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.2675f, 0.2680f, 0.2695f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.225f, 0.23f, 0.235f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		m_ItemColor.at(0) = ConvertFrVec4(colors[ImGuiCol_WindowBg]);
		m_ItemColor.at(1) = ConvertFrVec4(colors[ImGuiCol_Header]);
		m_ItemColor.at(2) = ConvertFrVec4(colors[ImGuiCol_HeaderHovered]);
		m_ItemColor.at(3) = ConvertFrVec4(colors[ImGuiCol_HeaderActive]);
		m_ItemColor.at(4) = ConvertFrVec4(colors[ImGuiCol_Button]);
		m_ItemColor.at(5) = ConvertFrVec4(colors[ImGuiCol_ButtonHovered]);
		m_ItemColor.at(6) = ConvertFrVec4(colors[ImGuiCol_ButtonActive]);
		m_ItemColor.at(7) = ConvertFrVec4(colors[ImGuiCol_FrameBg]);
		m_ItemColor.at(8) = ConvertFrVec4(colors[ImGuiCol_FrameBgHovered]);
		m_ItemColor.at(9) = ConvertFrVec4(colors[ImGuiCol_FrameBgActive]);
		m_ItemColor.at(10) = ConvertFrVec4(colors[ImGuiCol_Tab]);
		m_ItemColor.at(11) = ConvertFrVec4(colors[ImGuiCol_TabHovered]);
		m_ItemColor.at(12) = ConvertFrVec4(colors[ImGuiCol_TabActive]);
		m_ItemColor.at(13) = ConvertFrVec4(colors[ImGuiCol_TabUnfocused]);
		m_ItemColor.at(14) = ConvertFrVec4(colors[ImGuiCol_TabUnfocusedActive]);
		m_ItemColor.at(15) = ConvertFrVec4(colors[ImGuiCol_TitleBg]);
		m_ItemColor.at(16) = ConvertFrVec4(colors[ImGuiCol_TitleBgActive]);
		m_ItemColor.at(17) = ConvertFrVec4(colors[ImGuiCol_TitleBgCollapsed]);

	}

	void ImguiLayer::SetItemColor(const CostomItemColor& itemcolor)
	{
		m_ItemColor = itemcolor;

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ConvertImVec4(m_ItemColor.at(0));

		// Headers
		colors[ImGuiCol_Header] = ConvertImVec4(m_ItemColor.at(1));
		colors[ImGuiCol_HeaderHovered] = ConvertImVec4(m_ItemColor.at(2));
		colors[ImGuiCol_HeaderActive] = ConvertImVec4(m_ItemColor.at(3));

		// Buttons
		colors[ImGuiCol_Button] = ConvertImVec4(m_ItemColor.at(4));
		colors[ImGuiCol_ButtonHovered] = ConvertImVec4(m_ItemColor.at(5));
		colors[ImGuiCol_ButtonActive] = ConvertImVec4(m_ItemColor.at(6));

		// Frame BG
		colors[ImGuiCol_FrameBg] = ConvertImVec4(m_ItemColor.at(7));
		colors[ImGuiCol_FrameBgHovered] = ConvertImVec4(m_ItemColor.at(8));
		colors[ImGuiCol_FrameBgActive] = ConvertImVec4(m_ItemColor.at(9));

		// Tabs
		colors[ImGuiCol_Tab] = ConvertImVec4(m_ItemColor.at(10));
		colors[ImGuiCol_TabHovered] = ConvertImVec4(m_ItemColor.at(11));
		colors[ImGuiCol_TabActive] = ConvertImVec4(m_ItemColor.at(12));
		colors[ImGuiCol_TabUnfocused] = ConvertImVec4(m_ItemColor.at(13));
		colors[ImGuiCol_TabUnfocusedActive] = ConvertImVec4(m_ItemColor.at(14));

		// Title
		colors[ImGuiCol_TitleBg] = ConvertImVec4(m_ItemColor.at(15));
		colors[ImGuiCol_TitleBgActive] = ConvertImVec4(m_ItemColor.at(16));
		colors[ImGuiCol_TitleBgCollapsed] = ConvertImVec4(m_ItemColor.at(17));

	}
}