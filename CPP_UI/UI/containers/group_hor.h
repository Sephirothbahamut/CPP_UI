#pragma once

#include "common.h"

namespace UI::inner::containers
	{
	template <core::concepts::container container_t = core::container_own<0>>
	struct group_hor : public container_t
		{
		core::align_ver alignment{core::align_ver::top};

		virtual core::vec2f _get_size_min() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_min()};
				ret.x += element_val.x;
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}
		virtual core::vec2f _get_size_prf() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_prf()};
				ret.x += element_val.x;
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}
		virtual core::vec2f _get_size_max() const noexcept final override
			{
			core::vec2f ret{0, 0};
			for (const auto& element : container_t::elements_view)
				{
				core::vec2f element_val{element.get_size_max()};
				ret.x += element_val.x;
				ret.y = std::max(ret.y, element_val.y);
				}
			return ret;
			}

		virtual void on_resize() final override
			{
			std::vector<details::constraints_t> constraints; constraints.reserve(container_t::elements_view.size());
			for (const auto& element : container_t::elements_view) { constraints.emplace_back(details::constraints_t::hor(element)); }

			auto sizes{calc_sizes(container_t::rect.width(), constraints)};

			for (size_t i : utils::indices(container_t::elements_view))
				{
				auto& element{container_t::elements_view[i]};
				const auto& size{sizes[i]};
				
				element.resize({size, std::min<float>(element.get_size_max().y, container_t::rect.height())});
				}
			}
		virtual void on_reposition() noexcept
			{
			float x{0};
			for (auto& element : container_t::elements_view)
				{
				float y{0};
				switch (alignment)
					{
					case core::align_ver::top:    y = 0; break;
					case core::align_ver::middle: y = (container_t::rect.h() / 2.f) - (element.get_rect().h() / 2.f); break;
					case core::align_ver::bottom: y =  container_t::rect.h()        -  element.get_rect().h();        break;
					}

				element.reposition({container_t::rect.x() + x, container_t::rect.y() + y});
				x += element.get_rect().width();
				}
			};
		};
	}