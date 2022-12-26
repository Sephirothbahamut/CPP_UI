#pragma once

#include "common.h"

namespace UI::inner::widgets
	{
	struct spacer : core::widget
		{
		virtual core::widget_obs get_mouseover(utils::math::vec2f position) noexcept override { return nullptr; }

		core::vec2f _get_size_min() const noexcept final override { return {/**/     0,          0}; }
		core::vec2f _get_size_prf() const noexcept final override { return {/**/     0,          0}; }
		core::vec2f _get_size_max() const noexcept final override { return {core::finf, core::finf}; }
		};
	}