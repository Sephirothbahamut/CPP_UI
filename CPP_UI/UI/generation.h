#pragma once

#include "core/core.h"
#include "manager.h"

namespace UI::inner::generation
	{
	template <std::derived_from<core::element> T>
	class generator;
	template <std::derived_from<core::element> T>
	class spawner;

	template <std::derived_from<core::element> T>
	class spawner
		{
		template <std::derived_from<core::element> T>
		friend class UI::inner::generation::spawner;
		template <std::derived_from<core::element> T>
		friend class UI::inner::generation::generator;

		public:
			using value_type = T;

			utils::observer_ptr<T> operator->() noexcept { return element; }
			                   T& operator* () noexcept { return element; }

			template <std::derived_from<core::element> T, typename ...Args>
			requires std::is_base_of<core::owner, value_type>::value
				spawner<T> create(Args&&... args)
				{
				return {element, std::forward<Args>(args)...};
				}

			explicit operator bool() const noexcept { return element; }

		private:
			template <typename ...Args>
			spawner(core::owner_obs owner, Args&&... args)
				{
				auto own{std::make_unique<T>(std::forward<Args>(args)...)};
				element = own.get();
				owner->push(std::move(own));
				}

			utils::observer_ptr<T> element;
		};

	template <std::derived_from<core::element> T>
	class generator
		{
		public:
			using value_type = T;

			template <typename ...Args>
			generator(Args&&... args) : element{std::make_unique<T>(std::forward<Args>(args)...)} {}

			utils::observer_ptr<T> operator->() noexcept { return element.get(); }

			template <std::derived_from<core::element> T, typename ...Args>
			requires std::is_base_of<core::owner, value_type>::value
				spawner<T> create(Args&&... args)
				{
				if (!element) { throw std::runtime_error{"Cannot create new elements after the UI structure has been finalized."}; }
				return {core::owner_obs{element.get()}, std::forward<Args>(args)...};
				}

			manager finalize() noexcept { return {std::move(element)}; }

		private:
			std::unique_ptr<T> element;
		};
	}