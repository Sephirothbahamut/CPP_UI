#pragma once

#include <string>
#include <stdexcept>

#include <utils/containers/object_pool.h>
#include <utils/graphics/colour.h>

#include <d2d1.h>
#include "details/memory.h"

namespace graphics::d2d::brush
	{
	namespace details
		{
		template <typename T>
		class base
			{
			public:
				using value_type = T;
				T* operator->() const noexcept { return get(); }
				T* get() const noexcept { return d2d_brush_ptr; };
				~base() { d2d_brush_ptr->Release(); }

			protected:
				T* d2d_brush_ptr;
			};
		}
	}
namespace graphics::d2d
	{
	class render_target;
	class brushes;

	namespace concepts
		{
		template <typename T>
		concept brush = std::derived_from<T, graphics::d2d::brush::details::base<typename T::value_type>>;
		}
	}

namespace graphics::d2d::brush
	{
	class solid_color : public details::base<ID2D1SolidColorBrush>
		{
		friend class graphics::d2d::render_target;
		friend class graphics::d2d::brushes;

		public:
			solid_color(ID2D1HwndRenderTarget& render_target, utils::graphics::colour::rgba rgba) : rgba{rgba}
				{
				recreate(render_target);
				}

			void recreate(ID2D1HwndRenderTarget& render_target)
				{
				D2D1_BRUSH_PROPERTIES bp{.opacity{1}};
				HRESULT result = render_target.CreateSolidColorBrush(D2D1::ColorF{rgba.r, rgba.g, rgba.b, rgba.a}, bp, &d2d_brush_ptr);
				if (result != S_OK)
					{
					throw std::runtime_error{"Failed to create color brush. Error code: " + std::to_string(result)};
					}
				}

			bool operator==(const solid_color& oth) { return rgba == oth.rgba; }

		private:
			//store create info
			utils::graphics::colour::rgba rgba; 
		};
	}