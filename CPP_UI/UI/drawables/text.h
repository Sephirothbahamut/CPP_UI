#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::drawables
	{
	struct text : shape
		{
		std::string text;

		virtual void draw(graphics::d2d::render_target& rt) const noexcept override
			{
			}
		};
	}