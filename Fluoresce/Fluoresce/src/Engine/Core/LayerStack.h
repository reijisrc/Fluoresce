//==============================================================================//
// Name : LayerStack.h															// 
// Describe :	レイヤースタック												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Layer.h"

namespace Fluoresce {

	//	レイヤースタック
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32 m_LayerInsertIndex = 0;
	};
}