#pragma once
#include "audio_expression.h"
#include "audio_sub_object.h"

namespace Halley {
    class AudioSubObjectSwitch final : public IAudioSubObject {
    public:
        AudioSubObjectSwitch() = default;
        AudioSubObjectSwitch(const ConfigNode& node);
    	ConfigNode toConfigNode() const override;
    	
	    void load(const ConfigNode& node) override;

        AudioSubObjectType getType() override { return AudioSubObjectType::Switch; }
	    std::unique_ptr<AudioSource> makeSource(AudioEngine& engine, AudioEmitter& emitter) const override;

    	void loadDependencies(Resources& resources) override;
	    void serialize(Serializer& s) const override;
	    void deserialize(Deserializer& s) override;

    private:
		String switchId;
		HashMap<String, AudioSubObjectHandle> cases;
    };
}
