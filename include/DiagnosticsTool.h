#pragma once

#include "Access.h"
#include "DiagnosticsUtil.h"
#include "Tooling.h"

class DiagnosticsTool
	: public core::Tool
{
public:
	String get_menu_tool_name() override;
	void show_tool_window() override;

private:
	void update_fps();
	void show_fps();

	ScrollingBuffer fps_buffer;
	U32 current_fps;

	F32 current_time, prev_time;
};