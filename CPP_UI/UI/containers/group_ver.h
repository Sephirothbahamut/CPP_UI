#pragma once

#include "common.h"

namespace UI::inner::containers
	{
	template <core::concepts::container container_t = core::container_own<0>>
	struct group_ver : public container_t
		{
		core::align_hor alignment{core::align_hor::left};

		virtual core::vec2f _get_size_min() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_min()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}
		virtual core::vec2f _get_size_prf() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_prf()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}
		virtual core::vec2f _get_size_max() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_max()};
				ret.y += element_val.y;
				ret.x = std::max(ret.x, element_val.x);
				}
			return ret;
			}

		virtual void on_resize()  final override
			{
			std::vector<details::constraints_t> constraints; constraints.reserve(container_t::elements_view.size());
			for (const auto& element : container_t::elements_view) { constraints.emplace_back(details::constraints_t::ver(element)); }

			auto sizes{calc_sizes(container_t::rect.height(), constraints)};

			for (size_t i : utils::indices(container_t::elements_view))
				{
				auto& element{container_t::elements_view[i]};
				const auto& size{sizes[i]};
				
				element.resize({std::min<float>(element.get_size_max().x, container_t::rect.width()), size});
				}
			}
		virtual void on_reposition() noexcept
			{
			float y{0};
			for (auto& element : container_t::elements_view)
				{
				float x{0};
				switch (alignment)
					{
					case core::align_hor::left:   x = 0; break;
					case core::align_hor::center: x = (container_t::rect.w() / 2.f) - (element.get_rect().w() / 2.f); break;
					case core::align_hor::right:  x =  container_t::rect.w()        -  element.get_rect().w();        break;
					}

				element.reposition({container_t::rect.x() + x, container_t::rect.y() + y});
				y += element.get_rect().height();
				}
			};
		};
	}