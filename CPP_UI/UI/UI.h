#pragma once

#include <utils/compilation/warnings.h>

//#include "generation.h"
#include "containers.h"
#include "widgets.h"
#include "drawables.h"
#include "input.h"
#include "window.h"

namespace UI
	{
	using align_ver = inner::core::align_ver;
	using align_hor = inner::core::align_hor;

	namespace core { using namespace inner::core; }

	namespace containers
		{
		template <core::concepts::container container_t = core::container_own<0>> using group_hor = inner::containers::group_hor<container_t>;
		template <core::concepts::container container_t = core::container_own<0>> using group_ver = inner::containers::group_ver<container_t>;
		template <core::concepts::container container_t = core::container_own<0>> using overlay   = inner::containers::overlay  <container_t>;
		template <core::concepts::container container_t = core::container_own<0>> using one_of    = inner::containers::one_of   <container_t>;
		template <core::concepts::container container_t = core::container_own<1>> using padding   = inner::containers::padding  <container_t>;
		}
	namespace widgets 
		{
		using spacer      = inner::widgets::spacer     ;
		using dummy       = inner::widgets::dummy      ;
		using button      = inner::widgets::button     ;
		using toggle      = inner::widgets::toggle     ;
		using window_drag = inner::widgets::window_drag;
		//using tabbed      = inner::widgets::tabbed     ;
		}
	namespace drawables
		{
		using draw_shape_data = inner::drawables::draw_shape_data;
		using text            = inner::drawables::text           ;
		using rect            = inner::drawables::rect           ;
		}

	template <core::concepts::container container_t = core::container_own<1>>
	using input_wrapper = inner::input_wrapper<container_t>;

	using initializer   = inner::initializer;

	using window        = inner::window;
	
	namespace all
		{
		using namespace UI;
		using namespace containers;
		using namespace widgets;
		}
	}