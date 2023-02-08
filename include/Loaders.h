#pragma once

#include "Assets.h"
#include "Textures.h"
#include "Windowing.h"

class TextureLoader 
	: public core::AssetLoader<Texture>
	, public AccessStorage<Texture>
	, public MutAccessUnique<core::WindowingState>
	, public SignalProcessor<core::WindowShutdownSignal>
	, public SignalEmitter<core::AssetUnloadingSignal<Texture>>
{
	memory::RawPtr<Texture> load_texture(String);
	String get_default_asset_name() const override;
	void load_assets() override;
	ecs::Entity load_asset(String texture_name, String texture_path) override;

	void process_signal(core::WindowShutdownSignal&) override;

public:
	TextureLoader();

	Bool contains(String) const override;
};
