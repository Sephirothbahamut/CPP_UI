#pragma once

#include <string>
#include <stdexcept>

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
		public:
			solid_color(render_target& render_target, utils::graphics::colour::rgba rgba);
		};
	}