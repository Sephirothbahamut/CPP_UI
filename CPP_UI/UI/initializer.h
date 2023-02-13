#pragma once

#include <vector>
#include <optional>

#include "core/core.h"
#include "widgets/window_drag.h"

namespace UI::inner
	{
	class manager;

	class initializer
		{
		friend class manager;

		public:
			initializer(utils::MS::graphics::d2d::device_context context) : context_ptr{context}, debug_brushes { context } {}

		private:
			utils::MS::graphics::d2d::device_context context_ptr;
			core::debug_brushes debug_brushes;
		};
	}