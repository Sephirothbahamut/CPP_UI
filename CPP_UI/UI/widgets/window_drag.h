#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/math/geometry/circle.h>

#include "common.h"
#include "../window.h"

namespace UI::inner::widgets
	{
	struct window_drag : core::widget
		{
		window_drag() = default;
		window_drag(window& window) : window_ptr{&window} {}

		virtual void draw(graphics::d2d::render_target& rt) const noexcept
			{
			float radius = std::min(rect.width, rect.height) / 2.f;
			utils::math::geometry::circle circle{.center{rect.center}, .radius{radius}};
			utils::graphics::colour::rgba fill_colour{0, 0, 255, 50};

			rt.draw(circle, graphics::d2d::render_target::draw_shape_data{.fill_colour{fill_colour}});
			}

		virtual void on_mouse_button_down(utils::input::mouse::button button) override
			{
			if (window_ptr) { PostMessage(window_ptr->get_handle(), WM_SYSCOMMAND, SC_SIZE + 9/*undocumented SC_DRAG*/, 0); }
			}

		utils::observer_ptr<window> window_ptr;
		};
	}