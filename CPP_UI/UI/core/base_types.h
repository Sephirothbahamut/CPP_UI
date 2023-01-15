#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <stack>
#include <limits>
#include <type_traits>
#include <numeric>

#include <utils/math/vec2.h>
#include <utils/graphics/colour.h>

#include "../../graphics/d2d/render_target.h"

namespace UI::inner::core
	{
#pragma region Support
	using vec2f = utils::math::vec2f;
#pragma endregion Support

#pragma region Types
	class element;
	using elements_obs = std::vector<   utils::observer_ptr<element>>;
	using elements_own = std::vector<std::unique_ptr<element>>;
	using element_own  = std::unique_ptr<element>;
	using element_obs  = utils::observer_ptr<element>;
	using element_ref  = element&;

	class drawable;
	using drawables_obs = std::vector<   utils::observer_ptr<const drawable>>;
	using drawables_own = std::vector<std::unique_ptr<const drawable>>;
	using drawable_own  = std::unique_ptr<drawable>;
	using drawable_obs  = utils::observer_ptr<drawable>;
	using drawable_ref  = drawable;

	class owner;
	using owners_obs = std::vector<   utils::observer_ptr<owner>>;
	using owners_own = std::vector<std::unique_ptr<owner>>;
	using owner_own  = std::unique_ptr<owner>;
	using owner_obs  = utils::observer_ptr<owner>;
	using owner_ref  = owner&;

	class widget;
	using widgets_obs = std::vector<   utils::observer_ptr<widget>>;
	using widgets_own = std::vector<std::unique_ptr<widget>>;
	using widget_own  = std::unique_ptr<widget>;
	using widget_obs  = utils::observer_ptr<widget>;
	using widget_ref  = widget&;

	enum class orientation_t { hor, ver };
	enum class align_ver { top, middle, bottom };
	enum class align_hor { left, center, right };
#pragma endregion Types

#pragma region Constants
	inline const utils::graphics::colour::rgba elem_bg{0, 1, 1, .4};
	inline const utils::graphics::colour::rgba elem_br{0, 1, 1, .8};
	inline const utils::graphics::colour::rgba wrap_bg{1, 1, 0, .4};
	inline const utils::graphics::colour::rgba wrap_br{1, 1, 0, .8};
	inline const utils::graphics::colour::rgba cont_bg{1, 0, 0, .4};
	inline const utils::graphics::colour::rgba cont_br{1, 0, 0, .8};
	inline const utils::graphics::colour::rgba widg_bg{0, 1, 0, .4};
	inline const utils::graphics::colour::rgba widg_br{0, 1, 0, .8};
	inline const float finf{std::numeric_limits<float>::infinity()};
	inline const float fnan{std::numeric_limits<float>::quiet_NaN()};

	struct debug_brushes
		{
		debug_brushes(graphics::d2d::brushes& brushes) :
			elem_bg{brushes.create_solid(UI::inner::core::elem_bg)},
			elem_br{brushes.create_solid(UI::inner::core::elem_br)},
			wrap_bg{brushes.create_solid(UI::inner::core::wrap_bg)},
			wrap_br{brushes.create_solid(UI::inner::core::wrap_br)},
			cont_bg{brushes.create_solid(UI::inner::core::cont_bg)},
			cont_br{brushes.create_solid(UI::inner::core::cont_br)},
			widg_bg{brushes.create_solid(UI::inner::core::widg_bg)},
			widg_br{brushes.create_solid(UI::inner::core::widg_br)}
			{}

		graphics::d2d::brushes::handle elem_bg;
		graphics::d2d::brushes::handle elem_br;
		graphics::d2d::brushes::handle wrap_bg;
		graphics::d2d::brushes::handle wrap_br;
		graphics::d2d::brushes::handle cont_bg;
		graphics::d2d::brushes::handle cont_br;
		graphics::d2d::brushes::handle widg_bg;
		graphics::d2d::brushes::handle widg_br;
		};
#pragma endregion Constants

#pragma region inner_types

	class custom_sizes_t
		{
		public:
			custom_sizes_t() = default;
			custom_sizes_t(vec2f _min, vec2f _max)
				{
				if (_min.x > _max.x || _min.y > _max.y) { throw sizes_error; }
				this->_min = _min;
				this->_max = _max;
				}
			custom_sizes_t(vec2f _min, vec2f _prf, vec2f _max)
				{
				if (_min.x > _max.x || _min.y > _max.y || _prf.x < _min.x || _prf.x > _max.x || _prf.y < _min.y || _prf.y > _max.y) { throw sizes_error; }
				this->_min = _min;
				this->_max = _max;
				this->_prf = _prf;
				}

#pragma region Setters
			void set_min(vec2f val)
				{
				if ((!std::isnan(_prf.x) && val.x > _prf.x) ||
					(!std::isnan(_prf.y) && val.y > _prf.y) ||
					val.x > _max.x || val.y > _max.y)
					{
					throw sizes_error;
					}
				_min = val;
				}
			void set__prf(vec2f val)
				{
				if (val.x < _min.x || val.x > _max.x || val.y < _min.y || val.y > _max.y) { throw sizes_error; }
				_prf = val;
				}
			void set_max(vec2f val)
				{
				if ((!std::isnan(_prf.x) && val.x < _prf.x) ||
					(!std::isnan(_prf.y) && val.y < _prf.y) ||
					val.x < _min.x || val.y < _min.y)
					{
					throw sizes_error;
					}
				_max = val;
				}
			void set_min_x(float x)
				{
				if (!std::isnan(_prf.x)) { if (x > _prf.x) { throw sizes_error; } }
				else { if (x > _max.x) { throw sizes_error; } }
				_min.x = x;
				}
			void set_min_y(float y)
				{
				if (!std::isnan(_prf.y)) { if (y > _prf.y) { throw sizes_error; } }
				else { if (y > _max.y) { throw sizes_error; } }
				_min.y = y;
				}
			void set_prf_x(float x)
				{
				if (x < _min.x || x > _max.x) { throw sizes_error; }
				_prf.x = x;
				}
			void set_prf_y(float y)
				{
				if (y < _min.y || y > _max.y) { throw sizes_error; }
				_prf.y = y;
				}
			void set_max_x(float x)
				{
				if (!std::isnan(_prf.x)) { if (x < _prf.x) { throw sizes_error; } }
				else { if (x < _min.x) { throw sizes_error; } }
				_max.x = x;
				}
			void set_max_y(float y)
				{
				if (!std::isnan(_prf.y)) { if (y < _prf.y) { throw sizes_error; } }
				else { if (y < _min.y) { throw sizes_error; } }
				_max.y = y;
				}
#pragma endregion Setters
#pragma region Getters
			const vec2f get_prf() const noexcept { return _prf; }
			const vec2f get_min() const noexcept { return _min; }
			const vec2f get_max() const noexcept { return _max; }
			/**/  float get_min_x() const noexcept { return _min.x; }
			/**/  float get_min_y() const noexcept { return _min.y; }
			/**/  float get_prf_x() const noexcept { return _prf.x; }
			/**/  float get_prf_y() const noexcept { return _prf.y; }
			/**/  float get_max_x() const noexcept { return _max.x; }
			/**/  float get_max_y() const noexcept { return _max.y; }
#pragma endregion Getters
#pragma region Properties
			__declspec(property(get = get_min, put = set_min)) vec2f min;
			__declspec(property(get = get_prf, put = set_prf)) vec2f prf;
			__declspec(property(get = get_max, put = set_max)) vec2f max;

			__declspec(property(put = set_min_x)) float min_x;
			__declspec(property(put = set_min_y)) float min_y;
			__declspec(property(put = set_prf_x)) float prf_x;
			__declspec(property(put = set_prf_y)) float prf_y;
			__declspec(property(put = set_max_x)) float max_x;
			__declspec(property(put = set_max_y)) float max_y;
#pragma endregion Properties

		private:
			inline static const std::runtime_error sizes_error{"Trying to assign invalid custom sizes. The following must be true for both x and y: _min <= _max."};

			vec2f _min{/*****/0,    0}; //can be [0, _max]
			vec2f _max{/**/finf, finf}; //can be [_min, finf]
			vec2f _prf{/**/fnan, fnan}; //can be [_min, _max] or fnan
		};
#pragma endregion inner_types
	}