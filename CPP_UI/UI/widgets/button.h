#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/input/mouse.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::widgets
	{
	class button : public core::widget_wrapper
		{
		public:
			button(std::function<void()> callback) : callback{callback} {}

			virtual void on_focus_lose () override { _pressed = false; }
			virtual void on_mouse_enter() override { _hovered = true ; }
			virtual void on_mouse_leave() override { _hovered = false; }

			virtual void on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) override
				{
				if(id == utils::input::mouse::button_id::left)
					{
					if (_hovered)
						{
						if (state) { _pressed = true; return; }
						else if (_pressed) { callback(); }
						}
					}
				_pressed = false; 
				}

			std::function<void()> callback;

		private:
			bool _pressed{false};
			bool _hovered{false};
		};
	}