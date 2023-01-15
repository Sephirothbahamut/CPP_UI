#pragma once

#include <vector>
#include <optional>

#include "core/core.h"
#include "../graphics/d2d/brush.h"
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
			manager(core::element_own& root, utils::input::mouse& mouse, graphics::d2d::render_target& rt, graphics::d2d::brushes& brushes) :
				root{std::move(root)},
				drawables{this->root->get_drawables()},
				rt_ptr{&rt},
				debug_brushes{brushes},
				input_mouse{*this, mouse}
				{
				}

			void draw() const noexcept 
				{
				for (const auto& drawable : drawables) 
					{ drawable->draw(*rt_ptr); } 
				}
			void debug_draw() const noexcept 
				{
				root->debug_draw(*rt_ptr, debug_brushes); 
				}
			void debug_drawz() const noexcept
				{
				root->debug_drawz(*rt_ptr, debug_brushes);
				}

			void resize(core::vec2f size) noexcept
				{
				root->resize    (size);
				root->reposition(root->rect.ul);
				}

			core::vec2f get_size_min() const noexcept { return root->get_size_min(); }
			core::vec2f get_size_max() const noexcept { return root->get_size_max(); }

		private:
			core::element_own root{nullptr};
			core::drawables_obs drawables;

			utils::observer_ptr<graphics::d2d::render_target> rt_ptr;
			core::debug_brushes debug_brushes;

#pragma region input
		private:
			core::widget_obs hover      {nullptr};
			core::widget_obs focus      {nullptr};
			//bool             drag_region{false  };

			class input_mouse
				{
				public:
					input_mouse(manager& manager, utils::input::mouse& mouse) :
						manager_ptr{&manager},
						handle_pos{mouse.position.on_changed.make_unique([this](const utils::math::vec2l& position, const utils::math::vec2l&           ) { moved (position ); })},
						handle_btn{mouse.buttons .on_changed.make_unique([this](const utils::input::mouse::button_id& id, const bool& state, const bool&) { button(id, state); })}
						{
						}

				private:
					utils::observer_ptr<manager> manager_ptr;

					decltype(utils::input::mouse::position)::callback_handle_unique handle_pos;
					decltype(utils::input::mouse::buttons )::callback_handle_unique handle_btn;


					void moved(utils::math::vec2l position)
						{
						core::widget_obs new_hover{manager_ptr->root->get_mouseover(position)};
						if (new_hover != manager_ptr->hover)
							{
							if (manager_ptr->hover)
								{
								manager_ptr->hover->on_mouse_leave();
								//if (dynamic_cast<utils::observer_ptr<UI::inner::widgets::window_drag>>(manager_ptr->hover))
								//	{
								//	manager_ptr->drag_region = false;
								//	}
								}
							if (new_hover)
								{
								new_hover->on_mouse_enter();
								//if (dynamic_cast<utils::observer_ptr<UI::inner::widgets::window_drag>>(new_hover))
								//	{
								//	manager_ptr->drag_region = true;
								//	}
								}
							manager_ptr->hover = new_hover;
							}
						}

					void button(const utils::input::mouse::button_id& id, const bool& state)
						{
						if (state)
							{
							if (manager_ptr->hover != manager_ptr->focus)
								{
								if (manager_ptr->focus) { manager_ptr->focus->on_focus_lose(); }
								if (manager_ptr->hover) { manager_ptr->hover->on_focus_gain(); }
								manager_ptr->focus = manager_ptr->hover;
								}
							if (manager_ptr->hover) { manager_ptr->focus->on_mouse_button(id, state); }
							}
						else
							{
							if (manager_ptr->hover) { manager_ptr->hover->on_mouse_button(id, state); }
							}
						}

				} input_mouse;
			friend class input_mouse;

#pragma endregion input
		};


	}