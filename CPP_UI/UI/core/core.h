#pragma once
#include <ranges>

#include <utils/math/geometry/aabb.h>
#include <utils/win32/window/input_system.h>
#include <utils/win32/window/regions.h>

#include "base_types.h"

namespace UI::inner::core
	{

	class element
		{
		public:
			utils::math::geometry::aabb rect;

			void debug_draw(graphics::d2d::render_target& rt) const noexcept
				{
				rt.draw(rect, graphics::d2d::render_target::draw_shape_data{.outline_colour{br()}, .outline_thickness{thickn()}, .fill_colour{bg()}});

				_debug_draw(rt);
				}

			virtual widget_obs    get_mouseover(vec2f position) noexcept { return nullptr; }
			
			virtual void draw(graphics::d2d::render_target& rt) const noexcept {};
			virtual drawables_obs get_drawables() const noexcept { return {}; }

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
				rect.size = size;
				}
			virtual void resize(vec2f max_size) { rect.size = max_size; };
			virtual void reposition(vec2f position) noexcept { rect.position = position; };

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

			virtual utils::graphics::colour::rgba bg() const noexcept = 0;
			virtual utils::graphics::colour::rgba br() const noexcept = 0;
			virtual float thickn() const noexcept = 0;
			virtual void _debug_draw(graphics::d2d::render_target& rt) const noexcept {}

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

	class owner : public element
		{
		public:
			virtual void push(std::unique_ptr<element>&& element) = 0;
		};

	class wrapper : public owner
		{
		public:
			virtual drawables_obs get_drawables() const noexcept final override { return element->get_drawables(); }
			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				return element->get_mouseover(position);
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
			virtual utils::graphics::colour::rgba bg() const noexcept final override { return wrap_bg; }
			virtual utils::graphics::colour::rgba br() const noexcept final override { return wrap_br; }
			virtual float thickn() const noexcept final override { return 2; }
			virtual void _debug_draw(graphics::d2d::render_target& rt) const noexcept final override { element->debug_draw(rt); }
		};

	class container : public owner
		{
		public:
			virtual drawables_obs get_drawables() const noexcept final override
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
			virtual utils::graphics::colour::rgba bg() const noexcept final override { return cont_bg; }
			virtual utils::graphics::colour::rgba br() const noexcept final override { return cont_br; }
			virtual float thickn() const noexcept final override { return 3; }
			virtual void _debug_draw(graphics::d2d::render_target& rt) const noexcept final override
				{
				for (const auto& element_ptr : elements)
					{
					element_ptr->debug_draw(rt);
					}
				}
		};

	class widget : public element
		{
		public:
			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				return rect.contains(position) ? this : nullptr;
				}

			virtual drawables_obs get_drawables() const noexcept final override { return {this}; }

			virtual void on_focus_gain () {}
			virtual void on_focus_lose () {}
			virtual void on_mouse_enter() {}
			virtual void on_mouse_leave() {}
			virtual void on_mouse_button_down(utils::input::mouse::button button) {}
			virtual void on_mouse_button_up  (utils::input::mouse::button button) {}

		private:
			virtual utils::graphics::colour::rgba bg() const noexcept final override { return widg_bg; }
			virtual utils::graphics::colour::rgba br() const noexcept final override { return widg_br; }
			virtual float thickn() const noexcept final override { return 1; }
		};
	}