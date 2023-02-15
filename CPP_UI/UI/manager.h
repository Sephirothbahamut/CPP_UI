#pragma once

#include <vector>
#include <optional>

#include "core/core.h"
#include "initializer.h"
#include "widgets/window_drag.h"

#include <utils/MS/window/regions.h>
#include <utils/input/mouse.h>

namespace UI::inner { class window; }
namespace UI::inner::widgets { struct window_drag; }

namespace UI::inner
	{
	class manager
		{
		friend class window;

		public:
			manager(const initializer& initializer, core::element_own&& root, utils::input::mouse& mouse) :
				initializer_ptr{&initializer},
				root{std::move(root)},
				input_mouse{*this, mouse}
				{
				}

			void debug_draw(const utils::MS::graphics::d2d::device_context& context) const noexcept { root->debug_draw(context, initializer_ptr->debug_brushes); }
			void       draw(const utils::MS::graphics::d2d::device_context& context) const noexcept { root->      draw(context); }

			void resize(core::vec2f size) noexcept
				{
				root->resize    (size);
				root->reposition(root->get_rect().ul());
				}

			core::vec2f get_size_min() const noexcept { return root->get_size_min(); }
			core::vec2f get_size_max() const noexcept { return root->get_size_max(); }

			utils::observer_ptr<utils::MS::window::base> window_ptr{nullptr};

		private:
			utils::observer_ptr<const initializer> initializer_ptr{nullptr};
			core::element_own root{nullptr};

#pragma region input
		private:
			core::widget_obs hover{nullptr};
			core::widget_obs focus{nullptr};

			class input_mouse
				{
				public:
					input_mouse(manager& manager, utils::input::mouse& mouse) :
						manager_ptr{&manager},
						handle_pos{mouse.position.on_changed.make_unique([this](const utils::math::vec2l& position, const utils::math::vec2l&           ) { moved (position ); })},
						handle_lve{mouse.leave   .on_trigger.make_unique([this](                                                                        ) { leave (         ); })},
						handle_btn{mouse.buttons .on_changed.make_unique([this](const utils::input::mouse::button_id& id, const bool& state, const bool&) { button(id, state); })}
						{
						}

				private:
					utils::observer_ptr<manager> manager_ptr;

					decltype(utils::input::mouse::position)::callback_handle_unique handle_pos;
					decltype(utils::input::mouse::leave   )::callback_handle_unique handle_lve;
					decltype(utils::input::mouse::buttons )::callback_handle_unique handle_btn;

					void moved(utils::math::vec2l position)
						{
						bool redraw{false};

						core::widget_obs new_hover{manager_ptr->root->get_mouseover(position)};
						if (new_hover != manager_ptr->hover)
							{
							//if (new_hover) { std::cout << "UI mouseover: @" << new_hover << ", type " << typeid(*new_hover).name() << std::endl; }
							//else           { std::cout << "UI mouseover: @nullptr" << std::endl; }

							if (manager_ptr->hover)
								{
								redraw = manager_ptr->hover->on_mouse_leave();
								}
							if (new_hover)
								{
								redraw = new_hover->on_mouse_enter() || redraw;
								}
							manager_ptr->hover = new_hover;
							}

						if (redraw && manager_ptr->window_ptr)
							{
							RedrawWindow(manager_ptr->window_ptr->get_handle(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
							}
						}

					void leave()
						{
						bool redraw{false};

						if (manager_ptr->hover)
							{
							redraw = manager_ptr->hover->on_mouse_leave();
							manager_ptr->hover = nullptr;
							}

						if (redraw && manager_ptr->window_ptr)
							{
							RedrawWindow(manager_ptr->window_ptr->get_handle(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
							}
						}

					void button(const utils::input::mouse::button_id& id, const bool& state)
						{
						bool redraw{false};

						if (state)
							{
							if (manager_ptr->hover != manager_ptr->focus)
								{
								if (manager_ptr->focus) { redraw = manager_ptr->focus->on_focus_lose(); }
								if (manager_ptr->hover) { redraw = manager_ptr->hover->on_focus_gain(); }
								manager_ptr->focus = manager_ptr->hover;
								}
							if (manager_ptr->hover) { redraw = manager_ptr->focus->on_mouse_button(id, state); }
							}
						else
							{
							if (manager_ptr->focus) { redraw = manager_ptr->focus->on_mouse_button(id, state); }
							}

						if (redraw && manager_ptr->window_ptr)
							{
							RedrawWindow(manager_ptr->window_ptr->get_handle(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
							}
						}

				} input_mouse;
			friend class input_mouse;

#pragma endregion input
		};


	}