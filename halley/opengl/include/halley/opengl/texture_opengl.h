#pragma once

#include <halley/core/graphics/texture.h>
#include <halley/core/graphics/texture_descriptor.h>
#include <condition_variable>

namespace Halley
{
	class VideoOpenGL;
	enum class TextureFormat;

	class TextureOpenGL final : public Texture
	{
	public:
		explicit TextureOpenGL(VideoOpenGL& parent, Vector2i size);

		void bind(int textureUnit) override;
		void load(const TextureDescriptor& descriptor) override;
		bool isLoaded() const override;

	private:
		void loadImage(const char* px, size_t w, size_t h, size_t stride, TextureFormat format, bool useMipMap);
		unsigned int create(size_t w, size_t h, TextureFormat format, bool useMipMap, bool useFiltering);

		static unsigned int getGLFormat(TextureFormat format);

		VideoOpenGL& parent;
		GLsync fence = nullptr;

		std::atomic<bool> loaded;
		std::condition_variable loadWait;
		std::mutex loadMutex;
	};
}
