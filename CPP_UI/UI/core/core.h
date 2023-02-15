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
			const utils::math::geometry::aabb& get_rect() const noexcept { return rect; }

			void debug_draw_rect(const utils::MS::graphics::d2d::device_context& context, const utils::MS::graphics::d2d::solid_brush& bg, const utils::MS::graphics::d2d::solid_brush& br) const noexcept
				{
				D2D1_RECT_F d2d_rect{.left{rect.left}, .top{rect.up}, .right{rect.right}, .bottom{rect.bottom}};

				context->FillRectangle(d2d_rect, bg.get());

				context->DrawRectangle(d2d_rect, br.get(), 1);
				}

			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const debug_brushes& brushes) const noexcept
				{
				debug_draw_rect(context, brushes.elem_bg, brushes.elem_br);
				}
			virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept {}

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
				resize(size);
				}
			void resize    (vec2f max_size)          { rect.size    () = max_size; on_resize    (); };
			void reposition(vec2f position) noexcept { rect.position() = position; on_reposition(); };

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
			utils::math::geometry::aabb rect;

			virtual vec2f _get_size_min() const noexcept { return {0.f , 0.f }; }
			virtual vec2f _get_size_max() const noexcept { return {finf, finf}; }
			virtual vec2f _get_size_prf() const noexcept
				{
				vec2f min{get_size_min()};
				vec2f max{get_size_max()};
				return
					{
					max.x == finf ? std::max(128.f, min.x) : min.x + ((max.x - min.x) / 2.f),
					max.y == finf ? std::max( 64.f, min.y) : min.y + ((max.y - min.y) / 2.f)
					};
				}

			virtual void on_resize    () {}
			virtual void on_reposition() {}
		};

	class owner : public virtual element
		{
		public:
			virtual void push(std::unique_ptr<element>&& element) = 0;
		};

	class wrapper : public owner
		{
		public:
			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(context, brushes.wrap_bg, brushes.wrap_br);
				if (element) { element->debug_draw(context, brushes); }
				}
			virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept override
				{
				if (element) { element->draw(context); }
				}

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
			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(context, brushes.cont_bg, brushes.cont_br);
				for (const auto& element : elements)
					{
					element->debug_draw(context, brushes);
					}
				}
			virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept override
				{
				for (const auto& element : elements)
					{
					element->draw(context);
					}
				}

			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				for (auto& element : elements)
					{
					if (widget_obs ret{element->get_mouseover(position)}) { return ret; }
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
			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(context, brushes.widg_bg, brushes.widg_br);
				}

			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				return rect.contains(position) ? this : nullptr;
				}

			virtual bool on_focus_gain () { return false; }
			virtual bool on_focus_lose () { return false; }
			virtual bool on_mouse_enter() { return false; }
			virtual bool on_mouse_leave() { return false; }
			virtual bool on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) { return false; }
		};

	}