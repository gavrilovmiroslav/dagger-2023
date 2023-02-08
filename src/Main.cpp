#pragma once

#include "Prelude.h"

using namespace core;

struct Player {};

struct ExitSystem
	: public ecs::System
	, public AccessUnique<InputSnapshot>
{
	void on_tick() override
	{
		if (access_unique().key_states[SDL_SCANCODE_ESCAPE])
		{
			Engine::get_instance().quit();
		}
	}
};

struct PlayerControlsSystem
	: public ecs::System
	, public AccessUnique<InputSnapshot>
	, public MutAccessGroupStorage<Player, Position>
{
	void on_tick() override
	{
		const auto& events = AccessUnique<InputSnapshot>::access_unique();		
		auto& players = MutAccessGroupStorage<Player, Position>::access_storage();

		for (const auto entity : players)
		{
			auto& position = players.get<Position>(entity);
			if (events.is_key(bus::Key::SDL_SCANCODE_LEFT))
			{
				position.inner.x--;
			} 
			else if (events.is_key(bus::Key::SDL_SCANCODE_RIGHT))
			{
				position.inner.x++;
			}
		}
	}
};

struct MyGame 
	: public Game
	, public AccessModule<AssetModule>
{
	MyGame()
	{
		auto& engine = Engine::get_instance();
		engine.use<PlayerControlsSystem>();
		engine.use<ExitSystem>();
	}

	void on_start() override
	{
		auto dagger = spawn()
			.with<Player>()
			.with<Position>(100, 100)
			.with<Visibility>(true)
			.with<Sprite>(
				access_module()->get_asset<Texture>("dagger.png"),
				geometry::Rect{ 0, 0, 2916, 8783 },
				geometry::Vec2{ 60, 176 })
			.done();

		replace_component<Position>(dagger, 200, 176);
	}
};

int wmain(int argc, char* argv[])
{
 	auto& engine = Engine::get_instance();
	engine.configure("dagger.ini", argc, argv);

	engine.setup<Default2D>();
	engine.setup<Editor>();

	MyGame game;
	engine.run();

	return 0;
}

