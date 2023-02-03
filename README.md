# Fluoresce

日本語 | [中国簡体字](README.zh-CN.md)

ECSアーキテクチャのゲームエンジン「Fluoresce」

## 概要
  `C++17` と `OpenGL4`で作成した軽量なゲームエンジン

## 動作環境
* OS
  * windows(x64)
* コンパイラー
  * msvc 15.0 以上

## ソリューション作成方法
  premakeでソリューションを生成します。

フォルダ内の `scripts/GenerateProject_VS2019_Win64` もしくは `scripts/GenerateProject_VS2019_Win64` をダブルクリックすれば、
ソリューションが生成されます

## 使用ライブラリ

下記のオープンソースライブラリを利用しています。

|ライブラリ|バージョン|ライセンス|
|:----|:----|:----|
|[Premake](https://github.com/premake/premake-core)| premake-5.0.0-beta2 |BSD 3-Clause|
|[spdlog](https://github.com/gabime/spdlog)| 1.110 |MIT License|
|[glad](https://github.com/Dav1dde/glad)| 2021-01 |MIT License|
|[glfw](https://github.com/glfw/glfw)| 3.3.8 |zlib/libpng license|
|[stb_image](https://github.com/nothings/stb)| 2.28 |無(Public Domain)|
|[Dear ImGui](https://github.com/ocornut/imgui)| 1.89 |MIT License|
|[ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)| v1.83 |MIT License|
|[entt](https://github.com/skypjack/entt)| v3.11.1 |MIT License|
|[yaml-cpp](https://github.com/jbeder/yaml-cpp)| 0.7.0 |MIT License|
|[Feather](https://feathericons.com)| v4.29.0 |MIT License|

[ライセンス (MIT)](https://github.com/reijisrc/Fluoresce/blob/main/LICENSE)
-------------------------------------------------------------------------------
	Copyright (c) 2023 Ding Qi.

	https://github.com/reijisrc/Fluoresce

    以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を取得するすべての人に対し、
    ソフトウェアを無制限に扱うことを無償で許可します。これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、
    および/または販売する権利、およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。

	上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。

    ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
    ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。
    作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、あるいはソフトウェアの使用
    またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。
