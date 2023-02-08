#pragma once

#include "Access.h"
#include "Tooling.h"

class DaggerEditorTool
	: public ecs::Module
	, public SignalProcessor<core::ToolRenderSignal>
	, public SignalEmitter<core::EditorToolAssetLoadingSignal>
	, public SignalEmitter<core::EditorToolMenuBarSignal>
{
	void process_signal(core::ToolRenderSignal&) override;
};