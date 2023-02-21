#pragma once

#include "common.h"
#include "overlay.h"

namespace UI::inner::containers
	{
	template <core::concepts::container container_t = core::container_own<0>>
	struct one_of : public overlay<container_t>
		{
		size_t current_index{0};

		virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const core::debug_brushes& brushes) const noexcept
			{
			overlay<container_t>::debug_draw_rect(context, brushes.cont_bg, brushes.cont_br);
			if (current_index < container_t::elements_view.size()) { container_t::elements_view[current_index].debug_draw(context, brushes); }
			}
		virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept 
			{
			if (current_index < container_t::elements_view.size()) { container_t::elements_view[current_index].draw(context); }
			}

		virtual core::widget_obs get_mouseover(core::vec2f position) noexcept 
			{
			if (current_index < container_t::elements_view.size()) { return container_t::elements_view[current_index].get_mouseover(position); }
			return nullptr; 
			}

		auto remove(core::element_obs element_ptr) noexcept
			{
			for (size_t i : utils::indices(container_t::elements_view))
				{
				const auto& element{container_t::elements_view[i]};
				if (std::addressof(element) == element_ptr)
					{
					if (i < current_index) { current_index--; }
					}
				}
			return overlay<container_t>::remove(element_ptr);
			}
		};
	}