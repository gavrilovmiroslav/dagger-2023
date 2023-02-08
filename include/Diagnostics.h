#pragma once

#include "Config.h"
#include "Definitions.h"

class Diagnostics {

public:
	static Diagnostics& get_instance()
	{
		static Diagnostics instance;
		return instance;
	}

	Diagnostics(Diagnostics const&) = delete;
	void operator=(Diagnostics const&) = delete;

	enum DiagType {
		DIAG_TYPE_FPS = 0,
		DIAG_TYPE_COUNT = 1
	};

	double get_diagnostics(DiagType diagType) const;

private:
	Diagnostics() = default;
};

