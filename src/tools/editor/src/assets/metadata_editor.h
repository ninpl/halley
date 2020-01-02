#pragma once
#include "halley/ui/ui_widget.h"

namespace Halley
{
	class Project;
	enum class AssetType;
	class UIFactory;

	class MetadataEditor : public UIWidget
	{
	public:
		MetadataEditor(UIFactory& factory);

		void setResource(Project& project, AssetType type, const String& name);

	private:
		UIFactory& factory;
		Metadata metadata;
		Project* project = nullptr;
		AssetType assetType;
		String assetId;

		void makeUI();

		void addInt2Field(const String& name, const String& x, const String& y, Vector2i defaultValue);
		void addInt4Field(const String& name, const String& x, const String& y, const String& z, const String& w, Vector4i defaultValue);
		void addStringField(const String& name, const String& key, const String& defaultValue);

		void makeLabel(const String& name);
		void makeIntField(UISizer& sizer, const String& key, int defaultValue);
		void makeStringField(UISizer& sizer, const String& key, const String& defaultValue);

		void saveMetadata();
	};
}
