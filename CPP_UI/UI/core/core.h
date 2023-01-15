#pragma once
#include <ranges>

#include <utils/math/geometry/aabb.h>
#include <utils/input/mouse.h>
#include <utils/MS/window/regions.h>

#include "base_types.h"

namespace UI::inner::core
	{
	class element
		{
		public:
			utils::math::geometry::aabb rect;

			void debug_draw_rect(graphics::d2d::render_target& rt, const graphics::d2d::brush::solid_color& bg, const graphics::d2d::brush::solid_color& br) const noexcept
				{
				D2D1_RECT_F d2d_rect{.left{rect.left}, .top{rect.up}, .right{rect.right}, .bottom{rect.bottom}};

				rt->FillRectangle(d2d_rect, bg.get());

				rt->DrawRectangle(d2d_rect, br.get(), 1);
				}

			virtual void debug_drawz(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept
				{
				}
			virtual void debug_draw(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept
				{
				debug_draw_rect(rt, brushes.elem_bg.value(), brushes.elem_br.value());
				}

			virtual drawables_obs get_drawables() const noexcept { return {}; };

			virtual widget_obs    get_mouseover(vec2f position) noexcept { return nullptr; }

			// Dynamic UI
			void resize_checked(vec2f size)
				{
				vec2f min_a = get_size_min();
				vec2f max_a = get_size_max();
				if (size.x < min_a.x /*|| size.x > max.x*/ ||
					size.y < min_a.y /*|| size.y > max.y*/)
					{
					//throw std::runtime_error{"Invalid resizing values."};
					using namespace utils::output;
					std::cout << "Invalid resizing values\n"
						<< "min: " << min_a	<< "\n"
						<< "val: " << size  << "\n"
						<< "max: " << max_a << "\n";
					}
				size.x = std::min(size.x, max_a.x);
				size.y = std::min(size.y, max_a.y);
				rect.size() = size;
				}
			virtual void resize(vec2f max_size) { rect.size() = max_size; };
			virtual void reposition(vec2f position) noexcept { rect.position() = position; };

			custom_sizes_t sizes;

			vec2f get_size_min() const noexcept { vec2f tmp{_get_size_min()}; return {std::max  (sizes.min.x,   tmp.x), std::max(sizes.min.y, tmp.y)}; }
			vec2f get_size_max() const noexcept { vec2f tmp{_get_size_max()}; return {std::min  (sizes.max.x,   tmp.x), std::min(sizes.max.y, tmp.y)}; }
			vec2f get_size_prf() const noexcept 
				{ 
				vec2f tmp{_get_size_prf()}; 
				vec2f min{get_size_min()};
				vec2f max{get_size_max()};

				return
					{
					std::isnan(sizes.prf.x) ? std::clamp(tmp.x, min.x, max.x) : sizes.prf.x, 
					std::isnan(sizes.prf.y) ? std::clamp(tmp.y, min.y, max.y) : sizes.prf.y
					};
				}

			inline constexpr operator bool() const noexcept { return true; }

		protected:
			virtual vec2f _get_size_min() const noexcept { return {0.f, 0.f}; }
			virtual vec2f _get_size_max() const noexcept { return {999.f, 999.f}; }
			virtual vec2f _get_size_prf() const noexcept
				{
				vec2f min{get_size_min()};
				vec2f max{get_size_max()};
				return
					{
					max.x == finf ? std::max(/**/128.f, min.x) : min.x + ((max.x - min.x) / 2.f),
					max.y == finf ? std::max(/***/64.f, min.y) : min.y + ((max.y - min.y) / 2.f)
					};
				}
		};
	class drawable : public virtual element
		{
		public:
			virtual void draw(graphics::d2d::render_target& rt) const noexcept {};
			virtual drawables_obs get_drawables() const noexcept final override { return {this}; }
		};

	class owner : public virtual element
		{
		public:
			virtual void push(std::unique_ptr<element>&& element) = 0;
		};

	class wrapper : public owner
		{
		public:
			virtual void debug_draw(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(rt, brushes.wrap_bg.value(), brushes.wrap_br.value());
				if (element) { element->debug_draw(rt, brushes); }
				}

			virtual drawables_obs get_drawables() const noexcept override { if (element) { return element->get_drawables(); } else { return {}; } }
			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				if (element) { return element->get_mouseover(position); }
				else { return nullptr; }
				}

			template <typename T, typename ...Args>
			T& emplace(Args&... args) noexcept 
				{ 
				std  ::  unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
				utils::observer_ptr<T> ptr{tmp.get()};
				element = std::move(tmp);
				return *ptr;
				}

			virtual void push(std::unique_ptr<element>&& element) noexcept final override { this->element = std::move(element); }

		protected:
			element_own element;
		};

	class container : public owner
		{
		public:
			virtual void debug_drawz(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept override
				{
				for (const auto& element_ptr : elements)
					{
					element_ptr->debug_draw(rt, brushes);
					}
				}
			virtual void debug_draw(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(rt, brushes.cont_bg.value(), brushes.cont_br.value());
				for (const auto& element_ptr : elements)
					{
					element_ptr->debug_draw(rt, brushes);
					}
				}

			virtual drawables_obs get_drawables() const noexcept override
				{
				drawables_obs ret;
				for (const auto& element_ptr : elements)
					{
					const auto& element = *element_ptr;
					drawables_obs tmp = element.get_drawables();
					ret.insert(ret.end(), tmp.begin(), tmp.end());
					}
				return ret;
				}
			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				for (auto& element_ptr : std::views::reverse(elements))
					{
					auto& element = *element_ptr;
					widget_obs tmp = element.get_mouseover(position);
					if (tmp) { return tmp; }
					}
				return nullptr;
				}

			template <typename T, typename ...Args>
			T& emplace(Args&&... args) noexcept
				{
				std::unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
				utils::observer_ptr<T> ptr{tmp.get()};
				elements.push_back(std::move(tmp));
				return *ptr;
				}

			virtual void push(std::unique_ptr<element>&& element) noexcept final override { this->elements.push_back(std::move(element)); }

		protected:
			elements_own elements;
		};
	
	class widget : public virtual element
		{
		public:
			virtual void debug_draw(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(rt, brushes.widg_bg.value(), brushes.widg_br.value());
				}

			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				return rect.contains(position) ? this : nullptr;
				}

			virtual void on_focus_gain () {}
			virtual void on_focus_lose () {}
			virtual void on_mouse_enter() {}
			virtual void on_mouse_leave() {}
			virtual void on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) {}

		protected:
		};

	class widget_wrapper : public widget, public wrapper
		{
		public:
			virtual void debug_draw(graphics::d2d::render_target& rt, const debug_brushes& brushes) const noexcept final override
				{
				widget::debug_draw(rt, brushes);
				}

			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				auto this_mouseover{widget::get_mouseover(position)};
				if (this_mouseover) { return this_mouseover; }
				return wrapper::get_mouseover(position);
				}

			virtual drawables_obs get_drawables() const noexcept final override
				{
				drawables_obs ret{widget ::get_drawables()};
				drawables_obs tmp{wrapper::get_drawables()};
				ret.insert(ret.end(), tmp.begin(), tmp.end());
				return ret; 
				}

		protected:
		};
	}