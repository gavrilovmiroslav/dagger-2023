#pragma once

#include "Access.h"
#include "Definitions.h"
#include "Geometry.h"

struct Texture;

struct Sprite
{
	ecs::Entity texture;
	geometry::Rect clip;
	geometry::Vec2 scale;
};

namespace core {
	struct WindowingState;
	struct RenderSignal;

	template<typename>
	struct AssetUnloadingSignal;
}

class SpriteRenderingModule
	: public ecs::Module
	, public AccessStorage<Texture>
	, public MutAccessStorage<Sprite>
	, public AccessGroupStorage<Sprite, Position, Visibility>
	, public AccessUnique<core::WindowingState>
	, public SignalProcessor<core::RenderSignal>
	, public SignalProcessor<core::AssetUnloadingSignal<Texture>>
{
	void process_signal(core::RenderSignal& signal) override;
	void process_signal(core::AssetUnloadingSignal<Texture>& signal) override;
};