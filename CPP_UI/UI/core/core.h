#pragma once
#include <ranges>
#include <algorithm>

#include <utils/math/geometry/aabb.h>
#include <utils/input/mouse.h>
#include <utils/MS/window/regions.h>

#include "base_types.h"

namespace UI::inner::core
	{
	class element
		{
		public:
			virtual ~element() {}

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

			vec2f get_size_min() const noexcept { vec2f tmp{_get_size_min()}; return {std::max(sizes.min.x, tmp.x), std::max(sizes.min.y, tmp.y)}; }
			vec2f get_size_max() const noexcept { vec2f tmp{_get_size_max()}; return {std::min(sizes.max.x, tmp.x), std::min(sizes.max.y, tmp.y)}; }
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

	template <typename VIEW_T>
		requires(std::same_as<core::element_ref, decltype(*(VIEW_T{}.begin()))>)
	class container_obs : public element
		{
		public:
			using view_t = VIEW_T;
			view_t elements_view;
			
			virtual void debug_draw(const utils::MS::graphics::d2d::device_context& context, const debug_brushes& brushes) const noexcept override
				{
				debug_draw_rect(context, brushes.cont_bg, brushes.cont_br);
				for (const auto& element : elements_view)
					{
					element->debug_draw(context, brushes);
					}
				}
			virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept override
				{
				for (const auto& element : elements_view)
					{
					element->draw(context);
					}
				}

			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				for (auto& element : elements_view)
					{
					if (widget_obs ret{element->get_mouseover(position)}) { return ret; }
					}
				return nullptr;
				}

		private:
		};

	namespace details
		{
		template <size_t SLOTS_COUNT = 0>
		class container_own_inner
			{
			public:
				static constexpr size_t slots_count{SLOTS_COUNT};
		
			public:

				using elements_t = std::conditional_t
						<
						is_vector,
						std::vector<element_own>, 
						std::array<element_own, slots_count>
						>;
				elements_t elements;

			protected:
				static constexpr bool is_vector{is_owning && slots_count == 0};
				static constexpr bool is_single{is_owning && slots_count == 1};
				static constexpr bool is_array {is_owning && slots_count >= 1};

				core::element_ref extract_element(core::element_own& element_ptr) { return *element_ptr; }
				auto make_extract_view() { return elements | std::views::transform(&extract_element); }
				using extract_view_t = decltype(make_extract_view());
			};
		}

	template <size_t SLOTS_COUNT = 0>
	class container_own : public details::container_own_inner<SLOTS_COUNT>, public container_obs<typename details::container_own_inner<SLOTS_COUNT>::extract_view_t>
		{
		protected:
			using container_own_t = details::container_own_inner<SLOTS_COUNT>;
			using container_obs_t = container_obs<typename container_own_t::extract_view_t>;
			using container_own_t::is_vector;
			using container_own_t::is_single;
			using container_own_t::is_array ;

		public:
			container_own() : container_obs_t{container_own_t::make_extract_view()}
				{}

			void update_view() noexcept
				{
				container_obs_t::elements_view = container_own_t::make_extract_view();
				}
		
			template <typename T, typename ...Args>
			T& emplace(Args&&... args) noexcept
				requires(is_vector || is_single)
				{
				return push<T>(std::make_unique<T>(std::forward<Args>(args)...));
				}

			template <typename T, typename ...Args>
			T& emplace_at(size_t index, Args&&... args) noexcept
				requires(is_vector || is_array)
				{
				if constexpr (is_vector) { return insert<T>(index, std::make_unique<T>(std::forward<Args>(args)...)); }
				if constexpr (is_array ) { return set   <T>(index, std::make_unique<T>(std::forward<Args>(args)...)); }
				}
				
			template <typename T = core::element>
			T& push(std::unique_ptr<T>&& element) noexcept
				requires(is_vector || is_single)
				{
				utils::observer_ptr<T> ptr{element.get()};

				     if constexpr (is_vector) { elements.push_back(std::move(element)); }
				else if constexpr (is_single) { elements[0] = std::move(element); }

				update_view();
				return *ptr;
				}
				
			template <typename T = core::element>
			T& set(size_t index, std::unique_ptr<T>&& element) noexcept
				requires(is_array)
				{
				utils::observer_ptr<T> ptr{element.get()};

				elements[index] = std::move(element);

				return *ptr;
				}

			template <typename T = core::element>
			T& insert(size_t index, std::unique_ptr<T>&& element) noexcept
				requires(is_vector)
				{
				utils::observer_ptr<T> ptr{element.get()};

				elements.insert(elements.begin() + index, std::move(element));

				update_view();
				return *ptr;
				}

			core::element_own remove(core::element_obs element) noexcept
				requires(is_vector)
				{
				auto it{std::find_if(elements.begin(), elements.end(), [this, element](const core::element_own& ptr) -> bool { return ptr.get() == element; })};
				if (it != elements.end())
					{
					auto ret{std::move(*it)};
					     if constexpr (is_vector) { elements.erase(it); }
					else if constexpr (is_array ) { *it = nullptr; }

					update_view();
					return ret;
					}

				update_view();
				return nullptr;
				}
		};

	namespace concepts
		{
		template <typename T>
		concept container_own = std::derived_from<T, UI::inner::core::container_own<T::slots_count>>;
		template <typename T>
		concept container_obs = std::derived_from<T, UI::inner::core::container_obs<typename T::view_t>>;

		template <typename T>
		concept container = container_own<T> || container_obs<T>;
		}

	struct widget
		{
		virtual bool on_focus_gain () { return false; }
		virtual bool on_focus_lose () { return false; }
		virtual bool on_mouse_enter() { return false; }
		virtual bool on_mouse_leave() { return false; }
		virtual bool on_mouse_button(const utils::input::mouse::button_id& id, const bool& state) { return false; }
		};

	class element_widget : public element, public widget
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

		};

	template <concepts::container container_t>
	class container_widget : public container_t, public widget
		{
		public:
			virtual widget_obs get_mouseover(vec2f position) noexcept override
				{
				return container_t::rect.contains(position) ? this : nullptr;
				}
		};

	}