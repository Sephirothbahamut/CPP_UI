#pragma once

#include "common.h"

namespace UI::inner::containers
	{
	struct group_ver : public core::container
		{
		core::align_hor alignment{core::align_hor::left};

		virtual core::vec2f _get_size_min() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_min()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}
		virtual core::vec2f _get_size_prf() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_prf()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}
		virtual core::vec2f _get_size_max() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element_ptr : elements)
				{
				core::vec2f element_val{element_ptr->get_size_max()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}

		virtual void on_resize()  final override
			{
			std::vector<details::constraints_t> constraints; constraints.reserve(elements.size());
			for (const auto& element_ptr : elements) { constraints.emplace_back(details::constraints_t::ver(*element_ptr)); }

			auto sizes{calc_sizes(rect.height(), constraints)};

			for (size_t i : utils::indices(elements))
				{
				auto& element{*elements[i]};
				const auto& size{sizes[i]};
				
				element.resize({std::min<float>(element.get_size_max().x, rect.width()), size});
				}

			//std::vector<constraints_t> constraints; constraints.reserve(elements.size());
			//for (const auto& element_ptr : elements)
			//	{
			//	const auto& element{*element_ptr};
			//	constraints.emplace_back(constraints_t::ver(element));
			//	}
			//auto sizes{calc_sizes({rect.height(), rect.width()}, constraints)};
			//
			//for (size_t i : utils::indices(elements))
			//	{
			//	auto& element{*elements[i]};
			//	const auto& size{sizes[i]};
			//	element.resize({size.second, size.first});
			//	}
			}
		virtual void on_reposition() noexcept
			{
			float y{0};
			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};
				float x{0};
				switch (alignment)
					{
					case core::align_hor::left:   x = 0; break;
					case core::align_hor::center: x = (rect.w() / 2.f) - (element.get_rect().w() / 2.f); break;
					case core::align_hor::right:  x =  rect.w()        -  element.get_rect().w();        break;
					}

				element.reposition({rect.x() + x, rect.y() + y});
				y += element_ptr->get_rect().height();
				}
			};
		};
	}