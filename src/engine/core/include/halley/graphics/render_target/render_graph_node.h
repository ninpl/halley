#pragma once
#include <memory>
#include "halley/data_structures/vector.h"


#include "render_graph_definition.h"
#include "render_graph_pin_type.h"
#include "halley/graphics/texture_descriptor.h"

namespace Halley {
	class Material;
	class VideoAPI;
	class RenderContext;
	class Texture;
	class RenderGraph;
	class TextureRenderTarget;
	
	class RenderGraphNode {
		friend class RenderGraph;
	
	public:
		explicit RenderGraphNode(const RenderGraphNodeDefinition& definition);

	private:
		struct OtherPin {
			RenderGraphNode* node = nullptr;
			uint8_t otherId = 0;
		};

		struct InputPin {
			RenderGraphElementType type = RenderGraphElementType::Unknown;
			Vector<OtherPin> others;
			std::shared_ptr<Texture> texture;
		};

		struct OutputPin {
			RenderGraphElementType type = RenderGraphElementType::Unknown;
			Vector<OtherPin> others;
		};

		struct Variable {
			String name;
			ConfigNode value;
		};

		void connectInput(uint8_t inputPin, RenderGraphNode& node, uint8_t outputPin);
		void disconnectInput(uint8_t inputPin);

		void startRender();
		void prepareDependencyGraph(VideoAPI& video, std::optional<Vector2i> targetSize);
		void prepareInputPin(InputPin& pin, VideoAPI& video, Vector2i targetSize);
		void prepareTextures(VideoAPI& video, const RenderContext& rc);
		
		void render(const RenderGraph& graph, VideoAPI& video, const RenderContext& rc, Vector<RenderGraphNode*>& renderQueue);
		void notifyOutputs(Vector<RenderGraphNode*>& renderQueue);

		void resetTextures();
		std::shared_ptr<Texture> makeTexture(VideoAPI& video, RenderGraphElementType type);
		void updateTexture(std::shared_ptr<Texture> texture, RenderGraphElementType type);

		void determineIfCanForwardRenderTarget();
		void determineIfNeedsRenderTarget();
		
		void renderNode(const RenderGraph& graph, const RenderContext& rc);
		void renderNodePaintMethod(const RenderGraph& graph, const RenderContext& rc);
		void renderNodeOverlayMethod(const RenderGraph& graph, const RenderContext& rc);
		void renderNodeImageOutputMethod(const RenderGraph& graph, const RenderContext& rc);
		void renderNodeBlitTexture(std::shared_ptr<const Texture> texture, const RenderContext& rc);
		RenderContext getTargetRenderContext(const RenderContext& rc) const;
		std::shared_ptr<TextureRenderTarget> getRenderTarget(VideoAPI& video);

		String id;
		RenderGraphMethod method;

		String paintId;
		String cameraId;
		std::optional<Colour4f> colourClear;
		std::optional<float> depthClear;
		std::optional<uint8_t> stencilClear;

		std::shared_ptr<Material> overlayMethod;
		Vector<Variable> variables;
		
		bool activeInCurrentPass = false;
		int depsLeft = 0;
		Vector2i currentSize;

		Vector<InputPin> inputPins;
		Vector<OutputPin> outputPins;

		bool ownRenderTarget = false;
		bool canForwardRenderTarget = false;
		std::shared_ptr<TextureRenderTarget> renderTarget;
		RenderGraphNode* reuseRenderTarget = nullptr;
	};
}
