#pragma once

#include <iostream>

#include <utils/enum.h>
#include <utils/math/geometry/circle.h>

#include "common.h"

namespace UI::inner::drawables
	{
	struct rect : shape
		{
		virtual void draw(const utils::MS::graphics::d2d::device_context& context) const noexcept override
			{

			}
		};
	}