#include "Loaders.h"

#include "Assets.h"
#include "Definitions.h"
#include "Engine.h"

#include "Config.h"
#include "Logger.h"
#include "Rendering.h"
#include "SpriteRendering.h"
#include "Windowing.h"
#include "Textures.h"

#include <FPNG.h>
#include <LodePNG.h>
#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>
#include <algorithm>

// Texture loader

TextureLoader::TextureLoader()
	: core::AssetLoader<Texture>()
{
}

memory::RawPtr<Texture> TextureLoader::load_texture(String path)
{
	auto& state = MutAccessUnique<core::WindowingState>::access_unique();
	auto* texture = new Texture();

	U32 error;
	U32 width, height;
	U32 channelsInFile = 3;
	std::vector<U8> data;

	auto filepath = path.c_str();
	auto result = fpng::fpng_decode_file(filepath, data, width, height, channelsInFile, 4);
	Logger::info("[FPNG] W: {}, H: {}, C: {}", width, height, channelsInFile);
	if (result == fpng::FPNG_DECODE_NOT_FPNG)
	{
		Logger::error("[FPNG] Decoder error: recoding into fpng", result);
		RawPtr<U8> ptr;

		error = lodepng_decode32_file(&ptr, &width, &height, filepath);
		Logger::info("[LODE] W: {}, H: {}, C: {}", width, height, channelsInFile);

		if (error) {
			Logger::error("[LODE] Decoder error {}: {}\n", error, lodepng_error_text(error));
			delete texture;
			return nullptr;
		}

		auto dataLength = width * height * sizeof(U32);
		Logger::info("[LODE] Expected: {}", dataLength);

		data.reserve(dataLength);
		for (int i = 0; i < dataLength; i++)
		{
			data.push_back(ptr[i]);
		}

		if (ptr != nullptr)
			free(ptr);

		fpng::fpng_encode_image_to_file(filepath, data.data(), width, height, channelsInFile, 0);
		Logger::info("[LODE] Recoding complete, will attempt to reload");

		result = fpng::fpng_decode_file(filepath, data, width, height, channelsInFile, 4);
		Logger::info("[FPNG] W: {}, H: {}, C: {}", width, height, channelsInFile);
	}
	else if (result != fpng::FPNG_DECODE_SUCCESS)
	{
		Logger::critical("[FPNG] Error: {}", result);
		delete texture;
		return nullptr;
	}

	Logger::info("Creating texture.");

	texture->inner = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	Logger::info("Creating underlying texture storage.");

	auto* pixels = (RawPtr<U32>)malloc(width * height * sizeof(U32));
	if (!pixels)
	{
		Logger::info("Failed to allocate pixels");
		delete texture;
		return nullptr;
	}

	Logger::info("Filling pixel data");
	for (U32 y = 0; y < height; y++)
	{
		for (U32 x = 0; x < width; x++)
		{
			auto fy = 4 * y * width;
			auto fx = 4 * x;
			auto fs = fx + fy;

			pixels[(y * width + x)] = 65536 * data[fs + 0] + 256 * data[fs + 1] + data[fs + 2];
		}
	}

	Logger::info("Updating texture");
	SDL_UpdateTexture(texture->inner, nullptr, pixels, width * sizeof(U32));

	Logger::info("Freeing texture storage");
	free(pixels);

	Logger::info("Loading complete");
	
	return texture;
}

String TextureLoader::get_default_asset_name() const
{
	return "textures";
}

ecs::Entity TextureLoader::load_asset(String texture_name, String texture_path)
{
	auto& registry = core::Engine::get_instance().registry;

	auto loaded = load_texture(texture_path);
	Bool is_found = contains(texture_name);

	if (loaded == nullptr)
	{
		if (is_found)
		{
			loaded_entity_mapping.erase(texture_name);
			Logger::error("Loading texture '{}' failed.", texture_path);
		}

		return ecs::no_entity;
	}

	if (loaded != nullptr)
	{
		if (!is_found)
		{
			auto entity = registry.create();
			registry.emplace<Texture>(entity, *loaded);
			loaded_entity_mapping.insert({ texture_name, entity });
			return entity;
		}
		else
		{
			auto entity = loaded_entity_mapping.at(texture_name);
			registry.replace<Texture>(entity, *loaded);
			return entity;
		}
	}

	return ecs::no_entity;
}

void TextureLoader::load_assets()
{
	auto& registry = core::Engine::get_instance().registry;
	auto asset_path = get_loader_path();
	auto yaml = YAML::LoadFile("data/" + asset_path);

	containers::DynamicArray<String> entries;
	containers::DynamicArray<String> to_remove;

	for (auto entry : yaml)
	{
		entries.push_back(entry.as<String>());
	}

	for (auto loaded : loaded_entity_mapping)
	{
		if (std::find(entries.begin(), entries.end(), loaded.first) == entries.end())
		{
			SignalEmitter<core::AssetUnloadingSignal<Texture>>::emit(core::AssetUnloadingSignal<Texture>{ loaded.second });
			registry.remove<Texture>(loaded.second);
		}

		to_remove.push_back(loaded.first);
	}

	for (auto del : to_remove)
	{
		loaded_entity_mapping.erase(del);
	}

	for (auto entry : yaml)
	{
		auto texture_name = entry.as<String>();
		auto texture_path = "data/" + entry.as<String>();

		load_asset(texture_name, texture_path);
	}
}

void TextureLoader::process_signal(core::WindowShutdownSignal&)
{
	auto& storage = AccessStorage<Texture>::access_storage();
	for (auto& e : storage)
	{
		auto& texture = storage.get<Texture>(e);
		SDL_DestroyTexture(texture.inner);
	}
}

Bool TextureLoader::contains(String name) const
{
	return loaded_entity_mapping.find(name) != loaded_entity_mapping.end();
}

// Other loaders
