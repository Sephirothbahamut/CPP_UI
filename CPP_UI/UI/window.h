#pragma once

#include "manager.h"

namespace UI::inner
	{
	//TODO move to CPP_Utilities
	template <std::integral T>
	T cast_clamp(float f)
		{
		if (f > static_cast<float>(std::numeric_limits<T>::max())) { return std::numeric_limits<T>::max(); }
		if (f < static_cast<float>(std::numeric_limits<T>::min())) { return std::numeric_limits<T>::min(); }
		return static_cast<T>(f);
		}

	class window : public virtual utils::win32::window::base
		{
		public:
			std::optional<LRESULT> procedure(UINT msg, WPARAM wparam, LPARAM lparam)
				{
				if (!ui_manager_ptr) { return std::nullopt; }

				switch (msg)
					{
					case WM_SIZE:
						ui_manager_ptr->resize(utils::math::vec2u{LOWORD(lparam), HIWORD(lparam)});
						break;

					case WM_NCHITTEST:
						return hit_test();

					case WM_GETMINMAXINFO:
						getminmaxinfo(lparam);
						return 0;
					}
				return std::nullopt;
				}

			void set_ui_manager(manager& manager) noexcept
				{
				ui_manager_ptr = &manager; 
				manager.resize(client_rect.size);
				}

			void enter_drag_region() noexcept { drag_region = true ; }
			void leave_drag_region() noexcept { drag_region = false; }

		private:
			utils::observer_ptr<UI::inner::manager> ui_manager_ptr{nullptr};
			bool drag_region{false};

			void getminmaxinfo(LPARAM lparam)
				{
				auto decorations_size{window_rect.size - client_rect.size};

				auto size_min{ui_manager_ptr->get_size_min() + decorations_size};
				auto size_max{ui_manager_ptr->get_size_max() + decorations_size};

				LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
				lpMMI->ptMinTrackSize.x = cast_clamp<LONG>(size_min.x);
				lpMMI->ptMinTrackSize.y = cast_clamp<LONG>(size_min.y);
				lpMMI->ptMaxTrackSize.x = cast_clamp<LONG>(size_max.x);
				lpMMI->ptMaxTrackSize.y = cast_clamp<LONG>(size_max.y);
				}
			std::optional<LRESULT> hit_test()
				{
				if (drag_region) { return utils::win32::window::hit_type::drag; }
				else { return std::nullopt; }
				}
		};
	}