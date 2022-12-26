#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::widgets
	{
	class button : public core::widget
		{
		public:
			button() = default;
			button(std::function<void()> callback) : callback{callback} {}

			virtual void draw(graphics::d2d::render_target& rt) const noexcept
				{
				float radius = std::min(rect.width, rect.height) / 2.f;
				utils::math::geometry::circle circle{.center{rect.center}, .radius{radius}};
				utils::graphics::colour::rgba fill_colour{0, 0, 255, 50};

				rt.draw(circle, graphics::d2d::render_target::draw_shape_data{.fill_colour{fill_colour}});
				}

			virtual void on_focus_gain () override { }
			virtual void on_focus_lose () override { _pressed = false; }
			virtual void on_mouse_enter() override { _hovered = true ; }
			virtual void on_mouse_leave() override { _hovered = false; }
			virtual void on_mouse_button_down(utils::input::mouse::button button) override
				{
				if (button != utils::input::mouse::button::left) { _pressed = false; }
				else if (_hovered) { _pressed = true; }
				}
			virtual void on_mouse_button_up  (utils::input::mouse::button button) override
				{
				if(button == utils::input::mouse::button::left && _hovered && _pressed)
					{
					if (callback) { callback(); }
					}
				_pressed = false;
				}

			std::function<void()> callback;

		private:
			bool _pressed;
			bool _hovered;
		};
	}