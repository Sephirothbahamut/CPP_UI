#pragma once
#include "../core/core.h"

namespace UI::inner::containers
	{
	struct padding_values
		{
#pragma region Variables
		float up{0};
		float ll{0};
		float dw{0};
		float rr{0};
#pragma endregion Variables

#pragma region Accessors
		// Sides
		float get_up()  const noexcept { return up; }
		float get_dw()  const noexcept { return dw; }
		float get_rr()  const noexcept { return rr; }
		float get_ll()  const noexcept { return ll; }
		void  set_up(float v) noexcept { up = v; }
		void  set_dw(float v) noexcept { dw = v; }
		void  set_rr(float v) noexcept { rr = v; }
		void  set_ll(float v) noexcept { ll = v; }
		core::vec2f get_tot() const noexcept { return {rr + ll, up + dw}; }
#pragma endregion Accessors

#pragma region Properties
		// Aliases
		__declspec(property(get = get_up, put = set_up)) float top;
		__declspec(property(get = get_dw, put = set_dw)) float bottom;
		__declspec(property(get = get_dw, put = set_dw)) float down;
		__declspec(property(get = get_ll, put = set_ll)) float left;
		__declspec(property(get = get_rr, put = set_rr)) float right;
		__declspec(property(get = get_tot)) core::vec2f tot;
#pragma endregion Properties
		};

	core::vec2f get_padding_aabb_tot(const utils::math::geometry::aabb& aabb) noexcept { return {aabb.rr + aabb.ll, aabb.up + aabb.dw}; }

	template <core::concepts::container container_t = core::container_own<1>>
	class padding : public container_t, public utils::math::geometry::aabb
		{
		public:
			padding(const utils::math::geometry::aabb& init) : utils::math::geometry::aabb{init} {}

			virtual core::vec2f _get_size_min() const noexcept final override { return container_t::elements_view[0].get_size_min() + get_padding_aabb_tot(*this); }
			virtual core::vec2f _get_size_prf() const noexcept final override { return container_t::elements_view[0].get_size_prf() + get_padding_aabb_tot(*this); }
			virtual core::vec2f _get_size_max() const noexcept final override { return container_t::elements_view[0].get_size_max() + get_padding_aabb_tot(*this); }

			virtual void on_resize()  final override
				{
				container_t::elements_view[0].resize(container_t::rect.size() - get_padding_aabb_tot(*this));
				}
			virtual void on_reposition() noexcept
				{
				container_t::elements_view[0].reposition(container_t::rect.position() + core::vec2f{left, top});
				};
		};

	template <core::concepts::container container_t = core::container_own<1>>
	class variable_padding : public container_t
		{
		public:
			utils::math::geometry::aabb min;
			utils::math::geometry::aabb prf;
			utils::math::geometry::aabb max;

			virtual core::vec2f _get_size_min() const noexcept final override { return container_t::elements_view[0].get_size_min() + get_padding_aabb_tot(min); }
			virtual core::vec2f _get_size_prf() const noexcept final override { return container_t::elements_view[0].get_size_prf() + get_padding_aabb_tot(prf); }
			virtual core::vec2f _get_size_max() const noexcept final override { return container_t::elements_view[0].get_size_max() + get_padding_aabb_tot(max); }

			//TODO
			virtual void on_resize()  final override
				{
				container_t::elements_view[0].resize(container_t::rect.size() - get_padding_aabb_tot(min));
				}
			virtual void on_reposition() noexcept
				{
				container_t::elements_view[0].reposition(container_t::rect.position() + core::vec2f{min.left, min.top});
				};
		};
	}