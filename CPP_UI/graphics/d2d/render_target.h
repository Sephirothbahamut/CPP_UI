#pragma once

#include <string>
#include <optional>
#include <stdexcept>
#include <unordered_map>

#include <utils/containers/object_pool.h>
#include <utils/math/rect.h>
#include <utils/math/geometry/aabb.h>
#include <utils/math/geometry/circle.h>
#include <utils/polymorphic_value.h>

#include <d2d1.h>

#include "brush.h"
#include "details/wrappers/factory.h"

inline std::wstring UTF8_to_wchar(const char* in)
	{
	std::wstring out;
	unsigned int codepoint;
	while (*in != 0)
		{
		unsigned char ch = static_cast<unsigned char>(*in);
		if (ch <= 0x7f)
			codepoint = ch;
		else if (ch <= 0xbf)
			codepoint = (codepoint << 6) | (ch & 0x3f);
		else if (ch <= 0xdf)
			codepoint = ch & 0x1f;
		else if (ch <= 0xef)
			codepoint = ch & 0x0f;
		else
			codepoint = ch & 0x07;
		++in;
		if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
			{
			if (sizeof(wchar_t) > 2)
				out.append(1, static_cast<wchar_t>(codepoint));
			else if (codepoint > 0xffff)
				{
				out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
				out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
				}
			else if (codepoint < 0xd800 || codepoint >= 0xe000)
				out.append(1, static_cast<wchar_t>(codepoint));
			}
		}
	return out;
	}

namespace graphics::d2d
	{
	class manager;
	class brushes
		{
		using container_t = utils::containers::object_pool<graphics::d2d::brush::solid_color, 8Ui64, false, false, true>;
		public:
			using handle = container_t::handle_shared;
			brushes(graphics::d2d::render_target& rt) :
				rt_ptr{&rt}
				{}

			handle create_solid(utils::graphics::colour::rgba rgba);

		private:
			utils::observer_ptr<graphics::d2d::render_target> rt_ptr;

			container_t container;
		};

	class render_target
		{
		public:
			render_target(const manager& manager, HWND hwnd) : brushes_solid{*this}
				{
				RECT rc;
				GetClientRect(hwnd, &rc);

				HRESULT result = manager.factory->CreateHwndRenderTarget
					(
					D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
					D2D1::HwndRenderTargetProperties
						(
						hwnd,
						D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
						),
					&ID2D1HwndRenderTarget_ptr
					);
				if (result != S_OK)
					{
					throw std::runtime_error{"Failed to create window's render target. Error code: " + std::to_string(result)};
					}
				}

			void invalidate() 
				{
				if (ID2D1HwndRenderTarget_ptr)
					{
					ID2D1HwndRenderTarget_ptr->Release();
					}
				ID2D1HwndRenderTarget_ptr = nullptr;
				}

			~render_target()
				{
				if (ID2D1HwndRenderTarget_ptr)
					{
					ID2D1HwndRenderTarget_ptr->Release();
					}
				}

			ID2D1HwndRenderTarget* operator->() const noexcept { return ID2D1HwndRenderTarget_ptr; }
			ID2D1HwndRenderTarget* get       () const noexcept { return ID2D1HwndRenderTarget_ptr; }

			brushes brushes_solid;

		private:
			ID2D1HwndRenderTarget* ID2D1HwndRenderTarget_ptr{nullptr};
		};

	brushes::handle inline brushes::create_solid(utils::graphics::colour::rgba rgba)
		{
		return container.make_shared(*rt_ptr->get(), rgba);
		}
	}