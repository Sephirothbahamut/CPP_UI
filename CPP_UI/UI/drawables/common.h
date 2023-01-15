#pragma once

#include "../core/core.h"

namespace UI::inner::drawables
	{
	struct shape : virtual core::element, core::drawable
		{
		struct draw_shape_data
			{
			utils::graphics::colour::rgba outline_colour   {1.f, 1.f, 1.f, 1.f};
			float                         outline_thickness{1};
			utils::graphics::colour::rgba fill_colour      {0.f, 0.f, 0.f, 1.f};
			} draw_shape_data;
		};
	}