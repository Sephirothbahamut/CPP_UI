#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::widgets
	{
	struct dummy : core::widget
		{
		virtual void draw(graphics::d2d::render_target& rt) const noexcept
			{
			float radius = std::min(rect.width, rect.height) / 2.f;
			utils::math::geometry::circle circle{.center{rect.center}, .radius{radius}};
			utils::graphics::colour::rgba fill_colour{0, 0, 255, 50};

			rt.draw(circle, graphics::d2d::render_target::draw_shape_data{.fill_colour{fill_colour}});
			}
		
		virtual void on_focus_gain () noexcept { std::cout << "on_focus_gain " << std::endl; }
		virtual void on_focus_lose () noexcept { std::cout << "on_focus_lose " << std::endl; }
		virtual void on_mouse_enter() noexcept { std::cout << "on_mouse_enter" << std::endl; }
		virtual void on_mouse_leave() noexcept { std::cout << "on_mouse_leave" << std::endl; }
		virtual void on_mouse_button_down(utils::input::mouse::button button) { std::cout << "on_mouse_button_down " << utils::magic_enum::enum_name(button) << std::endl; }
		virtual void on_mouse_button_up  (utils::input::mouse::button button) { std::cout << "on_mouse_button_up   " << utils::magic_enum::enum_name(button) << std::endl; }

		};
	}