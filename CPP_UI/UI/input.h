#pragma once

#include <vector>
#include <optional>

#include "core/core.h"
#include "initializer.h"

#include <utils/input/mouse.h>

namespace UI::inner
	{
	template <core::concepts::container container_t = core::container_own<1>>
	class input_wrapper : public container_t
		{
		public:
			input_wrapper(utils::input::mouse& mouse) :
				handle_pos{mouse.position.on_changed.make_unique([this](const utils::math::vec2l& position, const utils::math::vec2l&           ) { moved (position ); })},
				handle_lve{mouse.leave   .on_trigger.make_unique([this](                                                                        ) { leave (         ); })},
				handle_btn{mouse.buttons .on_changed.make_unique([this](const utils::input::mouse::button_id& id, const bool& state, const bool&) { button(id, state); })} {}
			

			virtual core::vec2f _get_size_min() const noexcept final override { return container_t::elements_view[0].get_size_min(); }
			virtual core::vec2f _get_size_prf() const noexcept final override { return container_t::elements_view[0].get_size_prf(); }
			virtual core::vec2f _get_size_max() const noexcept final override { return container_t::elements_view[0].get_size_max(); }

			virtual void on_resize()  final override
				{
				container_t::elements_view[0].resize(container_t::rect.size());
				}
			virtual void on_reposition() noexcept
				{
				container_t::elements_view[0].reposition(container_t::rect.position());
				};

		private:

		private:
			core::widget_obs hover{nullptr};
			core::widget_obs focus{nullptr};

#pragma region mouse
			decltype(utils::input::mouse::position)::callback_handle_unique handle_pos;
			decltype(utils::input::mouse::leave   )::callback_handle_unique handle_lve;
			decltype(utils::input::mouse::buttons )::callback_handle_unique handle_btn;

			void moved(utils::math::vec2l position)
				{
				bool redraw{false};

				core::widget_obs new_hover{container_t::elements_view[0].get_mouseover(position)};
				if (new_hover != hover)
					{
					//if (new_hover) { std::cout << "UI mouseover: @" << new_hover << ", type " << typeid(*new_hover).name() << std::endl; }
					//else           { std::cout << "UI mouseover: @nullptr" << std::endl; }

					if (hover)
						{
						redraw = hover->on_mouse_leave();
						}
					if (new_hover)
						{
						redraw = new_hover->on_mouse_enter() || redraw;
						}
					hover = new_hover;
					}

				if (redraw) { core::element::should_redraw = true; }
				}

			void leave()
				{
				bool redraw{false};

				if (hover)
					{
					redraw = hover->on_mouse_leave();
					hover = nullptr;
					}

				if (redraw) { core::element::should_redraw = true; }
				}

			void button(const utils::input::mouse::button_id& id, const bool& state)
				{
				bool redraw{false};

				if (state)
					{
					if (hover != focus)
						{
						if (focus) { redraw = focus->on_focus_lose(); }
						if (hover) { redraw = hover->on_focus_gain(); }
						focus = hover;
						}
					if (hover) { redraw = focus->on_mouse_button(id, state); }
					}
				else
					{
					if (focus) { redraw = focus->on_mouse_button(id, state); }
					}

				if (redraw) { core::element::should_redraw = true; }
				}
#pragma endregion mouse
		};


	}