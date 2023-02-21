#pragma once

#include <utils/compilation/warnings.h>

//#include "generation.h"
#include "containers.h"
#include "widgets.h"
#include "drawables.h"
#include "manager.h"
#include "window.h"

namespace UI
	{
	using align_ver = inner::core::align_ver;
	using align_hor = inner::core::align_hor;

	namespace core { using namespace inner::core; }

	namespace containers
		{
		template <size_t slots_count = 0, typename view_t = void> using group_hor = inner::containers::group_hor<slots_count, view_t>;
		template <size_t slots_count = 0, typename view_t = void> using group_ver = inner::containers::group_ver<slots_count, view_t>;
		template <size_t slots_count = 0, typename view_t = void> using overlay   = inner::containers::overlay  <slots_count, view_t>;
		template <size_t slots_count = 0, typename view_t = void> using one_of    = inner::containers::one_of   <slots_count, view_t>;
		template <                        typename view_t = void> using padding   = inner::containers::padding  <             view_t>;
		}
	namespace widgets 
		{
		using spacer      = inner::widgets::spacer     ;
		using dummy       = inner::widgets::dummy      ;
		using button      = inner::widgets::button     ;
		using toggle      = inner::widgets::toggle     ;
		using window_drag = inner::widgets::window_drag;
		using tabbed      = inner::widgets::tabbed     ;
		}
	namespace drawables
		{
		using draw_shape_data = inner::drawables::draw_shape_data;
		using text            = inner::drawables::text           ;
		using rect            = inner::drawables::rect           ;
		}
	//namespace generation
	//	{
	//	template <std::derived_from<inner::core::element> T>
	//	using generator = inner::generation::generator<T>;
	//
	//	template <std::derived_from<inner::core::element> T>
	//	using spawner = inner::generation::spawner<T>;
	//	}

	using initializer = inner::initializer;
	using manager     = inner::manager;
	using window      = inner::window;

	namespace all
		{
		using namespace UI;
		using namespace containers;
		using namespace widgets;
		}
	}