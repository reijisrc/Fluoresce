//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#include "EditorLayer.h"
#include "Engine/Graphics/GraphicsCore.h"

#include <imgui.h>

namespace Fluoresce {

	EditorLayer::EditorLayer() :
		Layer("EditorLayer")
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(DeltaTime ts)
	{
		static const FrVec4 clearColor = FrVec4(0.8f, 0.25f, 0.4f, 1.0f);

		RenderCommand::Clear();
		RenderCommand::SetClearColor(clearColor);
	}

	void EditorLayer::OnImGuiRender()
	{
		auto imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer)
		{
			imguiLayer->DockspaceBegin();

			ImGui::Begin("Test");
			ImGui::End();

			imguiLayer->DockspaceEnd();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{

	}
}