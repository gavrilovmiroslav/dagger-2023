#pragma once

#include "Assets.h"
#include "Engine.h"
#include "EventBus.h"
#include "Geometry.h"
#include "Loaders.h"
#include "Rendering.h"
#include "SpriteRendering.h"
#include "Windowing.h"
#include "Tooling.h"
#include "Textures.h"

#include "DaggerEditorTool.h"
#include "SpriteEditorTool.h"
#include "DataBrowserTool.h"
#include "DiagnosticsTool.h"
#include "ImagePreviewTool.h"
#include "TextEditorTool.h"

using namespace core;

struct Default2D
{
	Default2D()
	{
		auto& engine = Engine::get_instance();
		engine.use<WindowingModule>();
		engine.use<RenderingModule>();
		engine.use<EventBusModule>();

		auto& assets = engine.use<AssetModule>();
		assets->add_loader<TextureLoader>();

		engine.use<ToolModule>();

		engine.use<SpriteRenderingModule>();
	}
};

struct Editor
{
	Editor()
	{
		auto& engine = Engine::get_instance();

		engine.use<DaggerEditorTool>();
		engine.use<DiagnosticsTool>();
		engine.use<DataBrowserTool>();
		engine.use<SpriteEditorTool>();
		engine.use<ImagePreviewTool>();
		engine.use<TextEditorTool>();
	}
};