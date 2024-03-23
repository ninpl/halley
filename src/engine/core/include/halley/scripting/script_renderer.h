#pragma once
#include "script_node_type.h"
#include "script_node_enums.h"
#include "halley/graph/base_graph_renderer.h"
#include "halley/graphics/sprite/sprite.h"
#include "halley/maths/circle.h"
#include "halley/maths/vector2.h"

namespace Halley {
	class IScriptNodeType;
	class ScriptNodeTypeCollection;
	class World;
	class ScriptGraphNode;
	class ScriptGraph;
	class ScriptState;

	class ScriptRenderer : public BaseGraphRenderer {
	public:
		ScriptRenderer(Resources& resources, const World* world, const ScriptNodeTypeCollection& nodeTypeCollection, float nativeZoom);
		
		void setState(const ScriptState* scriptState);

		std::optional<NodeUnderMouseInfo> getNodeUnderMouse(Vector2f basePos, float curZoom, Vector2f mousePos, bool pinPriority) const override;
		NodeUnderMouseInfo getPinInfo(Vector2f basePos, float curZoom, GraphNodeId nodeId, GraphPinId pinId) const override;
		Vector2f getPinPosition(Vector2f basePos, const BaseGraphNode& node, GraphPinId idx, float zoom) const override;
		Vector<GraphNodeId> getNodesInRect(Vector2f basePos, float curZoom, Rect4f selBox) const override;
		void setDebugDisplayData(HashMap<int, String> values) override;

		static Colour4f getScriptNodeColour(const IScriptNodeType& nodeType);

	protected:
		bool isDimmed(GraphNodePinType type) const override;
		GraphPinSide getSide(GraphNodePinType pinType) const override;
		Colour4f getPinColour(GraphNodePinType pinType) const override;
		Colour4f getBaseNodeColour(const IGraphNodeType& type) const override;
		const IGraphNodeType* tryGetNodeType(const String& typeId) const override;

	private:		
		const World* world = nullptr;
		const ScriptNodeTypeCollection& nodeTypeCollection;
		
		const ScriptState* state = nullptr;

		Sprite nodeBg;
		Sprite nodeBgOutline;
		Sprite pinSprite;
		Sprite destructorBg;
		Sprite destructorIcon;
		TextRenderer labelText;
		HashMap<int, String> debugDisplayValues;

		void drawNodeOutputs(Painter& painter, Vector2f basePos, GraphNodeId nodeIdx, const BaseGraph& graph, float curZoom, float posScale) override;
		void drawNodeBackground(Painter& painter, Vector2f basePos, const BaseGraphNode& node, float curZoom, float posScale, NodeDrawMode drawMode) override;
		void drawNode(Painter& painter, Vector2f basePos, const BaseGraphNode& node, float curZoom, float posScale, NodeDrawMode drawMode, std::optional<GraphNodePinType> highlightElement, GraphPinId highlightElementId) override;

		NodeDrawMode getNodeDrawMode(GraphNodeId nodeId) const override;
		Vector2f getNodeSize(const IGraphNodeType& nodeType, const BaseGraphNode& node, float curZoom) const override;
		Vector2f getCommentNodeSize(const BaseGraphNode& node, float curZoom) const;
		Circle getNodeElementArea(const IGraphNodeType& nodeType, Vector2f basePos, const BaseGraphNode& node, size_t pinN, float curZoom, float posScale) const;

		String getDebugDisplayValue(uint16_t id) const;
	};
}
