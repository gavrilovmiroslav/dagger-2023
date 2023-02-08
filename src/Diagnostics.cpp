#include "Diagnostics.h"
#include "Engine.h"

double Diagnostics::get_diagnostics(DiagType diagType) const
{
	switch (diagType)
	{
		case DIAG_TYPE_FPS:
			return core::Engine::get_instance().get_fps();
	}
	return .0;
}