#pragma once

#include <vector>
#include <optional>

#include "core/core.h"
#include "widgets/dummy.h"

#include <utils/win32/window/regions.h>
#include <utils/win32/window/input_system.h>

namespace UI::inner { class window; }

namespace UI::inner
	{
	class manager
		{
		friend class window;

		public:
			manager() : manager{std::make_unique<widgets::dummy>()} {}
			manager(core::element_own&& root) : root{std::move(root)}, drawables{root ? root->get_drawables() : core::drawables_obs{}}
				{
				}
			void push(core::element_own&& root)
				{
				this->root = std::move(root);
				drawables = root ? root->get_drawables() : core::drawables_obs{};
				}

			void draw(graphics::d2d::render_target& rt) const noexcept 
				{
				for (const auto& drawable : drawables) 
					{ drawable->draw(rt); } 
				}
			void debug_draw(graphics::d2d::render_target& rt) const noexcept { root->debug_draw(rt); }


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

#pragma region input
		private:
			struct
				{
				decltype(utils::input::mouse::move_to_actions    )::handle_raw handle_move_to;
				decltype(utils::input::mouse::button_down_actions)::handle_raw handle_button_down;
				decltype(utils::input::mouse::button_up_actions  )::handle_raw handle_button_up;
				} mouse_events_handles;

			core::widget_obs hover{nullptr};
			core::widget_obs focus{nullptr};

			void mouse_move_to(utils::math::vec2l position)
				{
				core::widget_obs new_hover{root->get_mouseover(position)};
				if (new_hover != hover) 
					{
					if (hover) { hover->on_mouse_leave(); }
					if (new_hover) { new_hover->on_mouse_enter(); }
					hover = new_hover;
					}
				}
			void mouse_button_down(utils::input::mouse::button button)
				{
				if (hover != focus)
					{
					if (focus) { focus->on_focus_lose(); }
					if (hover) { hover->on_focus_gain(); }
					focus = hover;
					}
				if (hover) { focus->on_mouse_button_down(button); }
				}
			void mouse_button_up(utils::input::mouse::button button)
				{
				if (hover) { hover->on_mouse_button_up(button); }
				}

		public:
			void register_mouse_events(utils::input::mouse& mouse) noexcept
				{
				mouse_events_handles.handle_move_to     = mouse.move_to_actions    .emplace([this](utils::math::vec2l          position) { mouse_move_to    (position); });
				mouse_events_handles.handle_button_down = mouse.button_down_actions.emplace([this](utils::input::mouse::button button  ) { mouse_button_down(button  ); });
				mouse_events_handles.handle_button_up   = mouse.button_up_actions  .emplace([this](utils::input::mouse::button button  ) { mouse_button_up  (button  ); });
				}
#pragma endregion input
		};


	}