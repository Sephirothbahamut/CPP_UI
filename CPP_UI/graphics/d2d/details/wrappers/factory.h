#pragma once

#include <string>
#include <stdexcept>

#include <d2d1_1.h>
#include "../memory.h"

namespace graphics::d2d::details::wrappers
	{
	class factory
		{
		public:
			factory(D2D1_FACTORY_TYPE factory_type = D2D1_FACTORY_TYPE_SINGLE_THREADED)
				{
				HRESULT result = D2D1CreateFactory(factory_type, &D2DFactory_ptr);
				if (result != S_OK)
					{
					throw std::runtime_error{"Failed to create D2D factory. Error code: " + std::to_string(result)};
					}
				}

			~factory()
				{
				D2DFactory_ptr->Release();
				}

			ID2D1Factory1* operator->() const noexcept { return D2DFactory_ptr; }
			ID2D1Factory1* get       () const noexcept { return D2DFactory_ptr; }

		private:
			ID2D1Factory1* D2DFactory_ptr;
		};
	}