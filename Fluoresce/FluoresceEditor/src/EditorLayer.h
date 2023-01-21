//==============================================================================//
// Name : EditorLayer.h															// 
// Describe : 	�G�f�B�^�[���C���[												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// �G�f�B�^�[���C���[
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		void ImguiDockspaceBegin();
		void ImguiDockspaceEnd();
	};

};