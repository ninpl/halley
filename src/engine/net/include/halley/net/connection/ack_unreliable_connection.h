#pragma once

#include "iconnection.h"
#include "network_packet.h"
#include <memory>
#include "halley/data_structures/vector.h"
#include <chrono>
#include <deque>
#include <limits>
#include <cstdint>

namespace Halley
{
	class IAckUnreliableConnectionListener
	{
	public:
		virtual ~IAckUnreliableConnectionListener() {}

		virtual void onPacketAcked(int tag) = 0;
	};

	class IAckUnreliableConnectionStatsListener
	{
	public:
		virtual ~IAckUnreliableConnectionStatsListener() {}

		virtual void onPacketSent(uint16_t sequence) = 0;
		virtual void onPacketResent(uint16_t sequence) = 0;
		virtual void onPacketAcked(uint16_t sequence) = 0;
	};

	class AckUnreliableSubPacket
	{
	public:
		Vector<gsl::byte> data;
		int tag = -1;
		//bool reliable = false;
		bool resends = false;
		uint16_t seq = std::numeric_limits<uint16_t>::max();
		uint16_t resendSeq = 0;

		AckUnreliableSubPacket()
		{}

		AckUnreliableSubPacket(AckUnreliableSubPacket&& other) = default;

		AckUnreliableSubPacket(Vector<gsl::byte>&& data)
			: data(data)
			, resends(false)
		{}

		AckUnreliableSubPacket(Vector<gsl::byte>&& data, uint16_t resendSeq)
			: data(data)
			, resends(true)
			, resendSeq(resendSeq)
		{}
	};

	class AckUnreliableConnection : public IConnection
	{
		using Clock = std::chrono::steady_clock;

		struct SentPacketData
		{
			std::vector<int> tags;
			Clock::time_point timestamp;
			bool waiting = false;
		};

	public:
		AckUnreliableConnection(std::shared_ptr<IConnection> parent);

		void close() override;
		ConnectionStatus getStatus() const override;
		bool isSupported(TransmissionType type) const override;
		void send(TransmissionType type, OutboundNetworkPacket packet) override;
		bool receive(InboundNetworkPacket& packet) override;

		uint16_t sendTagged(gsl::span<const AckUnreliableSubPacket> subPackets);
		void addAckListener(IAckUnreliableConnectionListener& listener);
		void removeAckListener(IAckUnreliableConnectionListener& listener);

		float getLatency() const { return lag; }
		float getTimeSinceLastSend() const;
		float getTimeSinceLastReceive() const;

		void setStatsListener(IAckUnreliableConnectionStatsListener* listener);

	private:
		std::shared_ptr<IConnection> parent;

		uint16_t nextSequenceToSend = 0;
		uint16_t highestReceived = 0xFFFF;

		Vector<char> receivedSeqs; // 0 = not received, 1 = received
		Vector<SentPacketData> sentPackets;
		std::deque<InboundNetworkPacket> pendingPackets;

		Vector<IAckUnreliableConnectionListener*> ackListeners;
		IAckUnreliableConnectionStatsListener* statsListener = nullptr;

		float lag = 1; // Start at 1 second
		Clock::time_point lastReceive;
		Clock::time_point lastSend;

		void processReceivedPacket(InboundNetworkPacket& packet);
		unsigned int generateAckBits();

		void processReceivedAcks(uint16_t ack, unsigned int ackBits);
		bool onSeqReceived(uint16_t sequence);
		void onAckReceived(uint16_t sequence);
		void reportLatency(float lag);

		void notifySend(uint16_t sequence);
		void notifyResend(uint16_t sequence);
		void notifyAck(uint16_t sequence);
	};
}
