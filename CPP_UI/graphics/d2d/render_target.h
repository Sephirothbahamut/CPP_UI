#pragma once

#include <string>
#include <stdexcept>

#include <utils/math/rect.h>
#include <utils/math/geometry/aabb.h>
#include <utils/math/geometry/circle.h>
#include <utils/polymorphic_value.h>

#include <d2d1.h>

#include "../../graphics/d2d/brush.h"
#include "../../graphics/d2d/render_target.h"

#include "details/wrappers/factory.h"

namespace graphics::d2d
	{
	class manager;

	class render_target
		{
		public:
			render_target(const manager& manager, HWND hwnd)
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

			struct draw_shape_data
				{
				std::optional<utils::graphics::colour::rgba> outline_colour;
				std::optional<uint32_t> outline_thickness;
				std::optional<utils::graphics::colour::rgba> fill_colour;
				};

			//TODO cache brushes
			void draw(utils::math::geometry::aabb rect, draw_shape_data draw_shape_data)
				{
				D2D1_RECT_F d2d_rect{.left{rect.left}, .top{rect.up}, .right{rect.right}, .bottom{rect.bottom}};


				if (draw_shape_data.fill_colour)
					{
					graphics::d2d::brush::solid_color _brush{*this, draw_shape_data.fill_colour.value()};
					ID2D1HwndRenderTarget_ptr->FillRectangle(d2d_rect, _brush.get());
					}

				if (draw_shape_data.outline_colour && draw_shape_data.outline_thickness)
					{
					graphics::d2d::brush::solid_color _brush{*this, draw_shape_data.outline_colour.value()};

					ID2D1HwndRenderTarget_ptr->DrawRectangle(d2d_rect, _brush.get(), draw_shape_data.outline_thickness.value());
					}
				}
			void draw(utils::math::geometry::circle circle, draw_shape_data draw_shape_data)
				{
				D2D1_ELLIPSE d2d_ellipse{.point{.x{circle.center.x}, .y{circle.center.y}}, .radiusX{circle.radius}, .radiusY{circle.radius}};

				if (draw_shape_data.fill_colour)
					{
					graphics::d2d::brush::solid_color _brush{*this, draw_shape_data.fill_colour.value()};
					ID2D1HwndRenderTarget_ptr->FillEllipse(d2d_ellipse, _brush.get());
					}

				if (draw_shape_data.outline_colour && draw_shape_data.outline_thickness)
					{
					graphics::d2d::brush::solid_color _brush{*this, draw_shape_data.outline_colour.value()};

					ID2D1HwndRenderTarget_ptr->DrawEllipse(d2d_ellipse, _brush.get(), draw_shape_data.outline_thickness.value());
					}
				}

		private:
			ID2D1HwndRenderTarget* ID2D1HwndRenderTarget_ptr{nullptr};
		};
	}

namespace graphics::d2d::brush
	{
	inline solid_color::solid_color(render_target& render_target, utils::graphics::colour::rgba rgba)
		{
		D2D1_BRUSH_PROPERTIES bp{.opacity{1}};
		HRESULT result = render_target->CreateSolidColorBrush(D2D1::ColorF{rgba.r, rgba.g, rgba.b, rgba.a}, bp , &d2d_brush_ptr);
		if (result != S_OK)
			{
			throw std::runtime_error{"Failed to create color brush. Error code: " + std::to_string(result)};
			}
		}
	}