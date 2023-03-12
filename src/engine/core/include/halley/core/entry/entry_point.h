#pragma once

#include <memory>
#include "halley/data_structures/vector.h"
#include <cstdint>
#include "halley/core/game/main_loop.h"
#include "../version/version.h"

#ifdef _WIN32
#define HALLEY_STDCALL __stdcall
#else
#define HALLEY_STDCALL
#endif

namespace Halley
{
	class Game;
	class Core;
	class HalleyStatics;

	class IHalleyEntryPoint
	{
	public:
		virtual ~IHalleyEntryPoint() = default;

		virtual uint32_t getApiVersion() { return getHalleyDLLAPIVersion(); }
		virtual std::unique_ptr<Core> createCore(const Vector<std::string>& args) = 0;
		virtual std::unique_ptr<Game> createGame() = 0;
		virtual void initSharedStatics(const HalleyStatics& parent);
	};

	template <typename GameType>
	class HalleyEntryPoint final : public IHalleyEntryPoint
	{
	public:
		std::unique_ptr<Game> createGame() override
		{
			return std::make_unique<GameType>();
		}

		std::unique_ptr<Core> createCore(const Vector<std::string>& args) override
		{
			Expects(args.size() >= 1);
			Expects(args.size() < 1000);
			return std::make_unique<Core>(std::make_unique<GameType>(), args);
		}
	};
}
