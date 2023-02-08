#include "DiagnosticsTool.h"
#include "Diagnostics.h"

#include <imgui.h>
#include <implot.h>


String DiagnosticsTool::get_menu_tool_name()
{
	return "Diagnostics";
}

void DiagnosticsTool::show_tool_window()
{
	update_fps();
	show_fps();
}

void DiagnosticsTool::update_fps()
{
	current_time += ImGui::GetIO().DeltaTime;
	if (current_time - prev_time >= 1.0f)
	{
		current_fps = (U32)Diagnostics::get_instance().get_diagnostics(Diagnostics::DIAG_TYPE_FPS);
		fps_buffer.AddPoint(current_time, (F32)current_fps);
		prev_time = current_time;
	}
}

void DiagnosticsTool::show_fps()
{
	ImGui::Text("FPS: %d", current_fps);

	if (ImPlot::BeginPlot("##FPS")) {
		ImPlot::SetupAxisLimits(ImAxis_X1, current_time - 10.0, current_time, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 3000);

		if (fps_buffer.Data.size() > 0)
		{
			ImPlot::PlotLine(
				"", 
				&fps_buffer.Data[0].x, 
				&fps_buffer.Data[0].y, 
				(U32)fps_buffer.Data.size(),
				0,
				fps_buffer.offset,
				(U32)(2 * sizeof(float))
			);
		}

		ImPlot::EndPlot();
	}
}