#pragma once

#include "common.h"
#include "overlay.h"

namespace UI::inner::containers
	{
	template <size_t slots_count = 0, typename view_t = void>
	struct one_of : public overlay<slots_count, view_t>
		{
		using overlay<slots_count, view_t>::elements;
		using overlay<slots_count, view_t>::rect;
		using overlay<slots_count, view_t>::overlay;

		size_t current_index{0};

		virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const core::debug_brushes& brushes) const noexcept
			{
			overlay<slots_count, view_t>::debug_draw_rect(context, brushes.cont_bg, brushes.cont_br);
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

		auto remove(core::element_obs element) noexcept
			{
			for (size_t i = 0; i < elements.size(); i++)
				{
				if (elements[i] == element) 
					{
					if (i < current_index) { current_index--; }
					if (i == elements.size()) { i = elements.size() - 1; }
					}
				}
			return overlay<slots_count, view_t>::remove(element);
			}
		};
	}