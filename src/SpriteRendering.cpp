#include "SpriteRendering.h"

#include "Assets.h"
#include "Rendering.h"
#include "Textures.h"
#include "Windowing.h"

using namespace core;

void SpriteRenderingModule::process_signal(RenderSignal& signal)
{
	const auto& state = AccessUnique<WindowingState>::access_unique();

	const auto& group = AccessGroupStorage<Sprite, Position, Visibility>::access_storage();
	const auto& textures = AccessStorage<Texture>::access_storage();
	
	for (const auto entity : group)
	{		
		const auto& sprite = group.get<Sprite>(entity);

		if (sprite.texture == ecs::no_entity) continue;

		const auto& pos = group.get<Position>(entity);
		const auto& show = group.get<Visibility>(entity);

		if (show.shown)
		{
			geometry::Rect dest{ (int)pos.inner.x, (int)pos.inner.y, (int)sprite.scale.x, (int)sprite.scale.y };
			const auto& texture = textures.get<Texture>(sprite.texture);
			SDL_RenderCopy(state.renderer, texture.inner, &sprite.clip, &dest);
		}
	}
}

void SpriteRenderingModule::process_signal(AssetUnloadingSignal<Texture>& signal)
{
	const auto& sprites = MutAccessStorage<Sprite>::access_storage();

	for (const auto entity : sprites)
	{
		auto& sprite = sprites.get<Sprite>(entity);
		if (sprite.texture == signal.id)
			sprite.texture = ecs::no_entity;
	}
}