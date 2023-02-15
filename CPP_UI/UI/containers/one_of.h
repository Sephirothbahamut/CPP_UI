#pragma once

#include "common.h"
#include "overlay.h"

namespace UI::inner::containers
	{
	struct one_of : overlay
		{
		size_t current_index{0};

		virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const core::debug_brushes& brushes) const noexcept
			{
			debug_draw_rect(context, brushes.cont_bg, brushes.cont_br);
			if (current_index < elements.size()) { elements[current_index]->debug_draw(context, brushes); }
			}
		virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept 
			{
			if (current_index < elements.size()) { elements[current_index]->draw(context); }
			}

		virtual core::widget_obs get_mouseover(core::vec2f position) noexcept 
			{
			if (current_index < elements.size()) { return elements[current_index]->get_mouseover(position); }
			return nullptr; 
			}
		};
	}