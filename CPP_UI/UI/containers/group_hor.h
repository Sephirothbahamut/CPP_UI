#pragma once

#include "common.h"

namespace UI::inner::containers
	{
	struct group_hor : public core::container
		{
		core::align_ver alignment{core::align_ver::top};

		virtual core::vec2f _get_size_min() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_min()};
				ret.x += element_val.x;
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
				ret.x += element_val.x;
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
				ret.x += element_val.x;
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}

		virtual void on_resize() final override
			{
			std::vector<constraints_t> constraints; constraints.reserve(elements.size());
			for (const auto& element_ptr : elements)
				{
				const auto& element{*element_ptr};
				constraints.emplace_back(constraints_t::hor(element));
				}
			auto sizes{calc_sizes({rect.width(), rect.height()}, constraints)};

			for (size_t i : utils::indices(elements))
				{
				auto& element{*elements[i]};
				const auto& size{sizes[i]};
				element.resize({size.first, size.second});
				}
			}
		virtual void on_reposition() noexcept
			{
			float x{0};
			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};
				float y{0};
				switch (alignment)
					{
					case core::align_ver::top:    y = 0; break;
					case core::align_ver::middle: y = (rect.h() / 2.f) - (element.rect.h() / 2.f); break;
					case core::align_ver::bottom: y =  rect.h()        -  element.rect.h();        break;
					}

				element.reposition({rect.x() + x, rect.y() + y});
				x += element_ptr->rect.width();
				}
			};
		};
	}