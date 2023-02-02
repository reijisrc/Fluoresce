# Fluoresce

[日本語](README.md) | 简体中文

使用ECS(Entity Component System)架构的游戏引擎

## 简介
   基于 `C++17` 和 `OpenGL4`的小型游戏引擎

## 支持
* 操作系统
  * windows(64位)
* 编译器
  * msvc 15.0 或以上

## 构建项目
   本项目使用premake来生成解决方案

双击`scripts/GenerateProject_VS2019_Win64`或`scripts/GenerateProject_VS2019_Win64`即可生成解决方案

## 使用项目
本引擎使用了以下开源项目
|项目名|版本|协议|
|:----|:----|:----|
|[Premake](https://github.com/premake/premake-core)| premake-5.0.0-beta2 |BSD 3-Clause|
|[spdlog](https://github.com/gabime/spdlog)| 1.110 |MIT License|
|[glad](https://github.com/Dav1dde/glad)| 2021-01 |MIT License|
|[glfw](https://github.com/glfw/glfw)| 3.3.8 |zlib/libpng license|
|[stb_image](https://github.com/nothings/stb)| 2.28 |无|
|[Dear ImGui](https://github.com/ocornut/imgui)| 1.89 |MIT License|
|[ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)| v1.83 |MIT License|
|[entt](https://github.com/skypjack/entt)| v3.11.1 |MIT License|
|[yaml-cpp](https://github.com/jbeder/yaml-cpp)| 0.7.0 |MIT License|
|[Feather](https://feathericons.com)| v4.29.0 |MIT License|

[协议 (MIT)](https://github.com/reijisrc/Fluoresce/blob/main/LICENSE)
-------------------------------------------------------------------------------
	Copyright (C) 2023 Ding Qi. 保留所有版权.

	https://github.com/reijisrc/Fluoresce

	被授权人权利:
	被授权人有权利使用、复制、修改、合并、出版发行、散布、再授权及贩售软件及软件的副本。
	被授权人可根据程序的需要修改授权条款为适当的内容。

	被授权人义务:
	在软件和软件的所有副本中都必须包含版权声明和许可声明。由版权持有人及其他责任者“按原样”提供，包括
	但不限于商品的内在保证和特殊目的适用，将不作任何承诺，不做任何明示或暗示的保证。 在任何情况下，不
	管原因和责任依据，也不追究是合同责任、后果责任或侵权行为(包括疏忽或其它)，即使被告知发生损坏的可
	能性，在使用本软件的任何环节造成的任何直接、间接、偶然、特殊、典型或重大的损坏(包括但不限于使用替
	代商品的后果：使用、数据或利益的损失或业务干扰)，版权持有人、其他责任者或作者或所有者概不承担任何责任

	其他重要特性:
	此授权条款并非属Copyleft的自由软件授权条款，允许在自由/开放源码软件或非自由软件（proprietary software）所使用。
	MIT的内容可依照程序著作权者的需求更改内容。此亦为MIT与BSD（The BSD license, 3-clause BSD license）本质上不同处。
	MIT条款可与其他授权条款并存。另外，MIT条款也是自由软件基金会（FSF）所认可的自由软件授权条款，与GPL兼容
