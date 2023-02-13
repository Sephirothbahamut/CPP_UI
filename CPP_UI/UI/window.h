#pragma once

#include <utils/math/math.h>

#include "manager.h"

namespace UI::inner
	{

	class window : public utils::MS::window::module
		{
		public:
			struct create_info
				{
				using module_type = window;

				utils::observer_ptr<manager> manager_ptr{nullptr};
				};

			window(utils::MS::window::base& base, const create_info& create_info = {}) :
				module{base}, 
				ui_manager_ptr{create_info.manager_ptr}
				{}

			virtual utils::MS::window::procedure_result procedure(UINT msg, WPARAM wparam, LPARAM lparam) override
				{
				if (ui_manager_ptr)
					{
					switch (msg)
						{
						case WM_SIZE:
							ui_manager_ptr->resize(utils::math::vec2u{LOWORD(lparam), HIWORD(lparam)});
							return utils::MS::window::procedure_result::next(0);

						//case WM_NCHITTEST:
							//	return hit_test();

						case WM_GETMINMAXINFO:
							getminmaxinfo(lparam);
							return utils::MS::window::procedure_result::stop(0);
						}
					}
				return utils::MS::window::procedure_result::next();
				}

			const manager& get_manager() const noexcept { return *ui_manager_ptr; }
			      manager& get_manager()       noexcept { return *ui_manager_ptr; }

			utils::observer_ptr<manager> ui_manager_ptr{nullptr};

		private:
			void getminmaxinfo(LPARAM lparam)
				{
				auto decorations_size{get_base().window_rect.size() - get_base().client_rect.size()};

				auto size_min{ui_manager_ptr->get_size_min() + decorations_size};
				auto size_max{ui_manager_ptr->get_size_max() + decorations_size};

				LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
				lpMMI->ptMinTrackSize.x = utils::math::cast_clamp<LONG>(size_min.x);
				lpMMI->ptMinTrackSize.y = utils::math::cast_clamp<LONG>(size_min.y);
				lpMMI->ptMaxTrackSize.x = utils::math::cast_clamp<LONG>(size_max.x);
				lpMMI->ptMaxTrackSize.y = utils::math::cast_clamp<LONG>(size_max.y);
				}
			//std::optional<LRESULT> hit_test()
			//	{
			//	if (ui_manager.drag_region) { return utils::MS::window::hit_type::drag; }
			//	else { return std::nullopt; }
			//	}
		};
	}