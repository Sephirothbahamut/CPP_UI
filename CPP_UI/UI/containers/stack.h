#pragma once

#include "common.h"

namespace UI::inner::containers
	{
	struct stack : public core::container
		{
		core::align_hor align_horment{core::align_hor::left};
		core::align_ver align_verment{core::align_ver::top};

		virtual core::vec2f _get_size_min() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_min()};
				ret.x = std::max(ret.x, element_val.x);
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}
		virtual core::vec2f _get_size_prf() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_prf()};
				ret.x = std::max(ret.x, element_val.x);
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}
		virtual core::vec2f _get_size_max() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_max()};
				ret.x = std::max(ret.x, element_val.x);
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}

		virtual void resize(core::vec2f max_size)  final override
			{
			core::element::resize_checked(max_size);

			for (size_t i : utils::indices(elements))
				{
				auto& element{*elements[i]};
				element.resize(rect.size);
				}
			}
		virtual void reposition(core::vec2f position) noexcept
			{
			core::element::reposition(position);

			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};

				float x{0};
				switch (align_horment)
					{
					case core::align_hor::left:   x = 0; break;
					case core::align_hor::center: x = (rect.w / 2.f) - (element.rect.w / 2.f); break;
					case core::align_hor::right:  x =  rect.w        - element.rect.w;         break;
					}
				float y{0};
				switch (align_verment)
					{
					case core::align_ver::top:    y = 0; break;
					case core::align_ver::middle: y = (rect.h / 2.f) - (element.rect.h / 2.f); break;
					case core::align_ver::bottom: y =  rect.h        - element.rect.h;         break;
					}

				element.reposition({rect.x + x, rect.y + y});
				}
			};
		};
	}