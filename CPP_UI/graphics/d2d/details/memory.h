#pragma once

namespace graphics::d2d::brush
	{
	template<class Interface>
	inline void SafeRelease(
		Interface** ppInterfaceToRelease)
		{
		if (*ppInterfaceToRelease != nullptr)
			{
			(*ppInterfaceToRelease)->Release();
			(*ppInterfaceToRelease) = nullptr;
			}
		}
	}