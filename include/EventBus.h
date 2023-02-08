#pragma once

#include "Config.h"
#include "Definitions.h"
#include "Engine.h"
#include "Logger.h"
#include "Access.h"
#include "Windowing.h"
#include <SDL2/SDL.h>

using namespace memory;

namespace core
{
	class ToolModule;

	enum MouseEventState
	{
		UpHold,
		UpNow,
		DownHold,
		DownNow,
	};

	struct BusEventSignal
	{
		SDL_Event event;
		Bool done_processing;
	};

	struct InputSnapshot
	{
		friend class EventBusModule;
		
	private:
		// mouse
		Pair<I32> mouse_position;
		U32 mouse_button_mask;
		Bool current_mouse_buttons[3];
		Bool previous_mouse_buttons[3];

		MouseEventState mouse_buttons[3]{ MouseEventState::UpHold, };

	public:
		// keyboard
		memory::RawPtr<const U8> key_states;

		inline MouseEventState get_mouse_button_left() const
		{			
			return mouse_buttons[0];
		}

		inline MouseEventState get_mouse_button_middle() const
		{
			return mouse_buttons[1];
		}

		inline MouseEventState get_mouse_button_right() const
		{
			return mouse_buttons[2];
		}

		inline Bool is_key(bus::Key code) const
		{
			return key_states[code];
		}
	};

	class EventBusModule
		: public ecs::Module
		, public AllocateUnique<InputSnapshot>
		, public AccessModule<ToolModule>
		, public MutAccessUnique<InputSnapshot>
		, public SignalEmitter<BusEventSignal>
		, SignalEmitter<core::WindowResizedSignal>
	{
	public:
		Bool on_start() override;

		void on_tick() override;
	};
}