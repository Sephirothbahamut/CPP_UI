#pragma once

#include <iostream>

#include <utils/enum.h>

#include "common.h"
#include <utils/MS/window/window.h>

namespace UI::inner::widgets
	{
	struct window_drag : core::widget
		{
		window_drag(utils::MS::window::base& window) : window_ptr{&window} {}
		
		virtual void on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) override
			{
			if (id == utils::input::mouse::button_id::left && state)
				{
				PostMessage(window_ptr->get_handle(), WM_SYSCOMMAND, SC_SIZE + 9/*undocumented SC_DRAG*/, 0);
				}
			}
		
		utils::observer_ptr<utils::MS::window::base> window_ptr;
		};
	}