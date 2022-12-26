#pragma once

#include <utils/graphics/colour.h>
#include <utils/win32/window/window.h>

#include "manager.h"
#include "render_target.h"

namespace graphics::d2d
	{
	class frame;

	class window : public virtual utils::win32::window::base
		{
		friend class frame;

		public:
			struct create_info
				{
				manager& manager;
				std::function<void(window&)> on_render;
				};

			window(create_info create_info) : 
				manager_ptr  {&create_info.manager},
				render_target{create_info.manager, get_handle()},
				on_render    {create_info.on_render}
				{
				}

			std::optional<LRESULT> procedure(UINT msg, WPARAM wparam, LPARAM lparam)
				{
				switch (msg)
					{
					case WM_SIZE:
						if (render_target.get())
							{
							render_target->Resize(D2D1::SizeU(LOWORD(lparam), HIWORD(lparam)));
							}
						break;

					case WM_DISPLAYCHANGE:
						//InvalidateRect(get_handle(), NULL, FALSE);
						break;

					case WM_PAINT:
						if (on_render)
							{
							on_render(*this);
							ValidateRect(get_handle(), NULL);
							return 0;
							}
						break;

					}

				return std::nullopt;
				}

			frame begin_frame(utils::graphics::colour::rgba clear_colour);

			std::function<void(window&)> on_render;

		private:
			utils::observer_ptr<manager> manager_ptr;
			render_target render_target;
		};

	class frame
		{
		friend class renderer;

		public:
			frame(const frame& copy) = delete;
			frame(frame&& move) = delete;
			frame& operator=(const frame& copy) noexcept = default;
			frame& operator=(frame&& move) noexcept = default;

			frame(window& window, manager& manager, utils::graphics::colour::rgba clear_colour) : window_ptr{&window}
				{
				if (!window_ptr->render_target.get())
					{
					window_ptr->render_target = d2d::render_target{manager, window.get_handle()};
					}

				window_ptr->render_target->BeginDraw();
				window_ptr->render_target->SetTransform(D2D1::Matrix3x2F::Identity());
				window_ptr->render_target->Clear(D2D1::ColorF(clear_colour.r, clear_colour.g, clear_colour.b, clear_colour.a));
				}

			~frame()
				{
				auto hr{window_ptr->render_target->EndDraw()};
				if (hr == D2DERR_RECREATE_TARGET) { window_ptr->render_target.invalidate(); }
				}

			render_target& rt() noexcept { return window_ptr->render_target; }

		private:
			utils::observer_ptr<window> window_ptr;
		};

	inline [[nodiscard]] frame window::begin_frame(utils::graphics::colour::rgba clear_colour)
		{
		return frame{*this, *manager_ptr, clear_colour};
		}
	}