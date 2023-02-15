#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/input/mouse.h>
#include <utils/math/geometry/circle.h>

#include "common.h"
#include "../containers/one_of.h"

namespace UI::inner::widgets
	{
	class button : public core::widget, public containers::one_of
		{
		public:
			button(std::function<void()> callback) : callback{callback} {}

			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const core::debug_brushes& brushes) const noexcept override
				{
				containers::one_of::debug_draw(context, brushes);
				}

			virtual core::widget_obs get_mouseover(core::vec2f position) noexcept override
				{
				return core::widget::get_mouseover(position);
				}

			virtual bool on_focus_lose () override { set_pressed(false); return true; }
			virtual bool on_mouse_enter() override { set_hovered(true ); return true; }
			virtual bool on_mouse_leave() override { set_hovered(false); return true; }
			
			virtual bool on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) override
				{
				on_mouse_button_inner(id, state);
				return true;
				}

			std::function<void()> callback;

		private:
			bool _pressed{false};
			bool _hovered{false};

			void on_mouse_button_inner(const utils::input::mouse::button_id& id, const bool& state)
				{
				if (id == utils::input::mouse::button_id::left)
					{
					if (_hovered)
						{
						if (state) { set_pressed(true); return; }
						else if (_pressed) { callback(); }
						}
					}
				set_pressed(false);
				}

			void set_pressed(bool state) noexcept
				{
				_pressed = state;
				update_index();
				}
			void set_hovered(bool state) noexcept
				{
				_hovered = state;
				update_index();
				}

			void update_index() noexcept
				{
				     if (_pressed) { if(elements.size()    ) { current_index = elements.size() - 1; } }
				else if (_hovered) { if(elements.size() > 1) { current_index = elements.size() - 2; } }
				else { current_index = 0; }
				}
		};
	}