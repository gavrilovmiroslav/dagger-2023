#pragma once

#include "Access.h"
#include "Assets.h"
#include "Tooling.h"

struct Texture;

class SpriteEditorTool
	: public core::Tool
	, public AccessModule<core::AssetModule>
	, public AccessStorage<Texture>
	, public SignalProcessor<core::EditorAssetSelectedIntent>
{
	String get_menu_tool_name() override;
	void show_tool_window() override;

	void process_signal(core::EditorAssetSelectedIntent& selection);

	String filepath;
public:

	SpriteEditorTool();

};