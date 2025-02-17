#include "halley/net/entity/entity_network_message.h"

#include <cassert>

#include "halley/bytes/byte_serializer.h"

using namespace Halley;

void EntityNetworkMessageCreate::serialize(Serializer& s) const
{
	s << entityId;
	s << bytes;
}

void EntityNetworkMessageCreate::deserialize(Deserializer& s)
{
	s >> entityId;
	s >> bytes;
}

void EntityNetworkMessageUpdate::serialize(Serializer& s) const
{
	s << entityId;
	s << bytes;
}

void EntityNetworkMessageUpdate::deserialize(Deserializer& s)
{
	s >> entityId;
	s >> bytes;
}

void EntityNetworkMessageDestroy::serialize(Serializer& s) const
{
	s << entityId;
}

void EntityNetworkMessageDestroy::deserialize(Deserializer& s)
{
	s >> entityId;
}

void EntityNetworkMessageReadyToStart::serialize(Serializer& s) const
{}

void EntityNetworkMessageReadyToStart::deserialize(Deserializer& s)
{}

void EntityNetworkMessageEntityMsg::serialize(Serializer& s) const
{
	s << entityUUID;
	s << messageType;
	s << messageData;
}

void EntityNetworkMessageEntityMsg::deserialize(Deserializer& s)
{
	s >> entityUUID;
	s >> messageType;
	s >> messageData;
}

void EntityNetworkMessageSystemMsg::serialize(Serializer& s) const
{
	s << messageType;
	s << msgId;
	s << wantsResponse;
	s << targetSystem;
	s << static_cast<int>(destination);
	s << messageData;
}

void EntityNetworkMessageSystemMsg::deserialize(Deserializer& s)
{
	s >> messageType;
	s >> msgId;
	s >> wantsResponse;
	s >> targetSystem;
	int d;
	s >> d;
	destination = static_cast<SystemMessageDestination>(d);
	s >> messageData;
}

void EntityNetworkMessageSystemMsgResponse::serialize(Serializer& s) const
{
	s << messageType;
	s << msgId;
	s << responseData;
}

void EntityNetworkMessageSystemMsgResponse::deserialize(Deserializer& s)
{
	s >> messageType;
	s >> msgId;
	s >> responseData;
}

void EntityNetworkMessageKeepAlive::serialize(Serializer& s) const
{}

void EntityNetworkMessageKeepAlive::deserialize(Deserializer& s)
{}

void EntityNetworkMessageJoinWorld::serialize(Serializer& s) const
{}

void EntityNetworkMessageJoinWorld::deserialize(Deserializer& s)
{}

void EntityNetworkMessageGetLobbyInfo::serialize(Serializer& s) const
{}

void EntityNetworkMessageGetLobbyInfo::deserialize(Deserializer& s)
{}

EntityNetworkMessageUpdateLobbyInfo::EntityNetworkMessageUpdateLobbyInfo(ConfigNode info)
	: lobbyInfo(std::move(info))
{
}

void EntityNetworkMessageUpdateLobbyInfo::serialize(Serializer& s) const
{
	s << lobbyInfo;
}

void EntityNetworkMessageUpdateLobbyInfo::deserialize(Deserializer& s)
{
	s >> lobbyInfo;
}

EntityNetworkMessageSetLobbyInfo::EntityNetworkMessageSetLobbyInfo(ConfigNode lobbyInfo)
	: lobbyInfo(std::move(lobbyInfo))
{
}

void EntityNetworkMessageSetLobbyInfo::serialize(Serializer& s) const
{
	s << lobbyInfo;
}

void EntityNetworkMessageSetLobbyInfo::deserialize(Deserializer& s)
{
	s >> lobbyInfo;
}

EntityNetworkMessage::EntityNetworkMessage(std::unique_ptr<IEntityNetworkMessage> msg)
	: message(std::move(msg))
{
}

void EntityNetworkMessage::setMessage(std::unique_ptr<IEntityNetworkMessage> msg)
{
	message = std::move(msg);
}

bool EntityNetworkMessage::needsWorld() const
{
	return message->needsWorld();
}

void EntityNetworkMessage::serialize(Serializer& s) const
{
	s << static_cast<int>(getType());
	message->serialize(s);
}

void EntityNetworkMessage::deserialize(Deserializer& s)
{
	int t;
	s >> t;
	const auto type = static_cast<EntityNetworkHeaderType>(t);

	switch (type) {
	case EntityNetworkHeaderType::Create:
		message = std::make_unique<EntityNetworkMessageCreate>();
		break;
	case EntityNetworkHeaderType::Update:
		message = std::make_unique<EntityNetworkMessageUpdate>();
		break;
	case EntityNetworkHeaderType::Destroy:
		message = std::make_unique<EntityNetworkMessageDestroy>();
		break;
	case EntityNetworkHeaderType::ReadyToStart:
		message = std::make_unique<EntityNetworkMessageReadyToStart>();
		break;
	case EntityNetworkHeaderType::EntityMsg:
		message = std::make_unique<EntityNetworkMessageEntityMsg>();
		break;
	case EntityNetworkHeaderType::SystemMsg:
		message = std::make_unique<EntityNetworkMessageSystemMsg>();
		break;
	case EntityNetworkHeaderType::SystemMsgResponse:
		message = std::make_unique<EntityNetworkMessageSystemMsgResponse>();
		break;
	case EntityNetworkHeaderType::KeepAlive:
		message = std::make_unique<EntityNetworkMessageKeepAlive>();
		break;
	case EntityNetworkHeaderType::JoinWorld:
		message = std::make_unique<EntityNetworkMessageJoinWorld>();
		break;
	case EntityNetworkHeaderType::GetLobbyInfo:
		message = std::make_unique<EntityNetworkMessageGetLobbyInfo>();
		break;
	case EntityNetworkHeaderType::UpdateLobbyInfo:
		message = std::make_unique<EntityNetworkMessageUpdateLobbyInfo>();
		break;
	case EntityNetworkHeaderType::SetLobbyInfo:
		message = std::make_unique<EntityNetworkMessageSetLobbyInfo>();
		break;
	}

	assert(message && message->getType() == type);

	message->deserialize(s);
}
