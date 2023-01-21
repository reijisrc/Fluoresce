//==============================================================================//
// Name : Layer.h																// 
// Describe :	レイヤー														// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Events/Event.h"

#include "Engine/Core/DeltaTime.h"

namespace Fluoresce {

	//	レイヤー
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(DeltaTime ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() { return m_LayerName; }
	private:
		std::string m_LayerName;
	};
}