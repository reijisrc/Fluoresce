//==============================================================================//
// Name : Components.h															// 
// Describe : 	�R���|�[�l���g													// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#pragma once

#include "Engine/Core/UniqueID.h"
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Graphics/Texture.h"

namespace Fluoresce {

	// ID�R���|�[�l���g
	struct IDComponent
	{
		UniqueID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	// �^�O�R���|�[�l���g
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	// �g�����X�t�H�[���R���|�[�l���g
	struct TransformComponent
	{
		Vec3 Translation = { 0.0f, 0.0f, 0.0f };
		Vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		Vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3& translation)
			: Translation(translation) {}

		Mat4 GetTransform() const;
	};

	// �J�����R���|�[�l���g
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;	// ���C���J�����t���O
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// �X�v���C�g�����_���[�R���|�[�l���g
	struct SpriteRendererComponent
	{
		Vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float32 TilingFactor = 1.0f;
		std::string		 TextureName;
		WeakRef<Texture2D>	 Texture;
		bool Visible = true;
		bool EnableTexture = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vec4& color)
			: Color(color) {}

		bool SetTextureAsset(const std::string& name);
	};

	class ScriptableEntity;

	// �X�N���v�g�R���|�[�l���g
	struct ScriptComponent
	{
		uint32	ScriptID = 0;
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(ScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](ScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

};
