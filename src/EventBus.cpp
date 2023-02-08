#include "EventBus.h"
#include "Engine.h"
#include "Tooling.h"

using namespace core;

bool EventBusModule::on_start()
{
	// TODO: add joystick recognition and support (see https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinput.html)
	// TODO: figure out how to enable adding and removing joysticks in runtime after start
	return true;
}

void EventBusModule::on_tick()
{
	bus::Event e;
	static BusEventSignal signal;

	while (bus::PollEvent(&e) != 0)
	{
		signal.event = e;
		SignalEmitter<BusEventSignal>::emit(signal);

		switch (e.type)
		{
		case SDL_QUIT:
			Engine::get_instance().quit();
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				SignalEmitter<core::WindowResizedSignal>::emit(core::WindowResizedSignal{ (U32)e.window.data1, (U32)e.window.data2 });
			}
			break;

		default:
			break;
		}
	}

	auto& state = MutAccessUnique<InputSnapshot>::access_unique();	
	state.mouse_button_mask = SDL_GetMouseState(&state.mouse_position.x, &state.mouse_position.y);

	for (U8 i = 1; i < 4; i++)
	{
		U8 j = i - 1;
		state.current_mouse_buttons[j] = state.mouse_button_mask & SDL_BUTTON(i);

		if (!state.previous_mouse_buttons[j] && !state.current_mouse_buttons[j])
		{
			state.mouse_buttons[j] = MouseEventState::UpHold;
		}
		else if (!state.previous_mouse_buttons[j] && state.current_mouse_buttons[j])
		{
			state.mouse_buttons[j] = MouseEventState::DownNow;
		}
		else if (state.previous_mouse_buttons[j] && state.current_mouse_buttons[j])
		{
			state.mouse_buttons[j] = MouseEventState::DownHold;
		}
		else if (state.previous_mouse_buttons[j] && !state.current_mouse_buttons[j])
		{
			state.mouse_buttons[j] = MouseEventState::UpNow;
		}

		state.previous_mouse_buttons[j] = state.current_mouse_buttons[j];
	}

	state.key_states = SDL_GetKeyboardState(nullptr);
}