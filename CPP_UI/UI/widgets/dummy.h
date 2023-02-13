#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/input/mouse.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::widgets
	{
	struct dummy : core::widget, core::drawable
		{
		virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept override
			{
			}
		
		virtual void on_focus_gain () override { std::cout << "on_focus_gain " << std::endl; }
		virtual void on_focus_lose () override { std::cout << "on_focus_lose " << std::endl; }
		virtual void on_mouse_enter() override { std::cout << "on_mouse_enter" << std::endl; }
		virtual void on_mouse_leave() override { std::cout << "on_mouse_leave" << std::endl; }
		virtual void on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) override 
			{
			std::cout << "on_mouse_button " << utils::enums::enum_name<utils::input::mouse::button_id>(id) << " " << (state ? "pressed" : "released") << std::endl;
			}
		};
	}