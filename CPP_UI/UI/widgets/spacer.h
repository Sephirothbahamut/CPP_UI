#pragma once

#include "common.h"

namespace UI::inner::widgets
	{
	struct spacer : core::widget
		{
		virtual void debug_draw(graphics::d2d::render_target& rt, const core::debug_brushes& brushes) const noexcept override
			{
			}

		virtual core::widget_obs get_mouseover(utils::math::vec2f position) noexcept override { return nullptr; }

		core::vec2f _get_size_min() const noexcept final override { return {/**/     0,          0}; }
		core::vec2f _get_size_prf() const noexcept final override { return {/**/     0,          0}; }
		core::vec2f _get_size_max() const noexcept final override { return {core::finf, core::finf}; }
		};
	}