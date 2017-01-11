#pragma once
#include "ui/ui_widget.h"
#include "graphics/sprite/sprite.h"
#include "api/audio_api.h"

namespace Halley {
	class UIStyle;

	class UIButton : public UIWidget {
		enum class State {
			Up,
			Down,
			Hover
		};

	public:
		explicit UIButton(String id, std::shared_ptr<UIStyle> style);

		void draw(UIPainter& painter) const override;
		void update(Time t, bool moved) override;
		bool isFocusable() const override;
		bool isFocusLocked() const override;

		void pressMouse(int button) override;
		void releaseMouse(int button) override;

	private:
		Sprite sprite;
		std::shared_ptr<UIStyle> style;
		bool held = false;
		State curState = State::Up;

		void playSound(const std::shared_ptr<const AudioClip>& clip);
		bool setState(State state);
	};
}
