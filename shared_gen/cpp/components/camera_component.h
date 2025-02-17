// Halley codegen version 128
#pragma once

#ifndef DONT_INCLUDE_HALLEY_HPP
#include <halley.hpp>
#else
#include "halley/entity/component.h"
#endif
#include "halley/support/exception.h"


class CameraComponent final : public Halley::Component {
public:
	static constexpr int componentIndex{ 6 };
	static const constexpr char* componentName{ "Camera" };

	float zoom{ 1 };
	Halley::String id{};
	Halley::Vector2f offset{};

	CameraComponent() {
	}

	CameraComponent(float zoom, Halley::String id, Halley::Vector2f offset)
		: zoom(std::move(zoom))
		, id(std::move(id))
		, offset(std::move(offset))
	{
	}

	Halley::ConfigNode serialize(const Halley::EntitySerializationContext& _context) const {
		using namespace Halley::EntitySerialization;
		Halley::ConfigNode _node = Halley::ConfigNode::MapType();
		Halley::EntityConfigNodeSerializer<decltype(zoom)>::serialize(zoom, float{ 1 }, _context, _node, componentName, "zoom", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
		Halley::EntityConfigNodeSerializer<decltype(id)>::serialize(id, Halley::String{}, _context, _node, componentName, "id", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
		Halley::EntityConfigNodeSerializer<decltype(offset)>::serialize(offset, Halley::Vector2f{}, _context, _node, componentName, "offset", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
		return _node;
	}

	void deserialize(const Halley::EntitySerializationContext& _context, const Halley::ConfigNode& _node) {
		using namespace Halley::EntitySerialization;
		Halley::EntityConfigNodeSerializer<decltype(zoom)>::deserialize(zoom, float{ 1 }, _context, _node, componentName, "zoom", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
		Halley::EntityConfigNodeSerializer<decltype(id)>::deserialize(id, Halley::String{}, _context, _node, componentName, "id", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
		Halley::EntityConfigNodeSerializer<decltype(offset)>::deserialize(offset, Halley::Vector2f{}, _context, _node, componentName, "offset", makeMask(Type::Prefab, Type::SaveData, Type::Dynamic, Type::Network));
	}

	Halley::ConfigNode serializeField(const Halley::EntitySerializationContext& _context, std::string_view _fieldName) const {
		using namespace Halley::EntitySerialization;
		if (_fieldName == "zoom") {
			return Halley::ConfigNodeHelper<decltype(zoom)>::serialize(zoom, _context);
		}
		if (_fieldName == "id") {
			return Halley::ConfigNodeHelper<decltype(id)>::serialize(id, _context);
		}
		if (_fieldName == "offset") {
			return Halley::ConfigNodeHelper<decltype(offset)>::serialize(offset, _context);
		}
		throw Halley::Exception("Unknown or non-serializable field \"" + Halley::String(_fieldName) + "\"", Halley::HalleyExceptions::Entity);
	}

	void deserializeField(const Halley::EntitySerializationContext& _context, std::string_view _fieldName, const Halley::ConfigNode& _node) {
		using namespace Halley::EntitySerialization;
		if (_fieldName == "zoom") {
			Halley::ConfigNodeHelper<decltype(zoom)>::deserialize(zoom, _context, _node);
			return;
		}
		if (_fieldName == "id") {
			Halley::ConfigNodeHelper<decltype(id)>::deserialize(id, _context, _node);
			return;
		}
		if (_fieldName == "offset") {
			Halley::ConfigNodeHelper<decltype(offset)>::deserialize(offset, _context, _node);
			return;
		}
		throw Halley::Exception("Unknown or non-serializable field \"" + Halley::String(_fieldName) + "\"", Halley::HalleyExceptions::Entity);
	}


	void* operator new(std::size_t size, std::align_val_t align) {
		return doNew<CameraComponent>(size, align);
	}

	void* operator new(std::size_t size) {
		return doNew<CameraComponent>(size);
	}

	void operator delete(void* ptr) {
		return doDelete<CameraComponent>(ptr);
	}

};
