#pragma once

#include <utils/memory.h>

#include <d3d11.h>
#include <d2d1.h>
#include <wrl.h>

#include "details/wrappers/factory.h"

namespace DX //no idea where to include this from so I made my own
	{
	// Helper class for COM exceptions
	class com_exception : public std::exception
		{
		public:
			com_exception(HRESULT hr) : result(hr) {}

			const char* what() const noexcept override
				{
				static char s_str[64] = {};
				sprintf_s(s_str, "Failure with HRESULT of %08X",
					static_cast<unsigned int>(result));
				return s_str;
				}

		private:
			HRESULT result;
		};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
		{
		if (FAILED(hr))
			{
			throw com_exception(hr);
			}
		}
	}

namespace graphics::d2d
	{
	class window;
	class render_target;
	class frame;

	class manager
		{
		friend class window;
		friend class render_target;
		friend class frame;

		public:
			manager()
				{
				// This flag adds support for surfaces with a different color channel ordering than the API default.
					// You need it for compatibility with Direct2D.
				UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

				// This array defines the set of DirectX hardware feature levels this app  supports.
				// The ordering is important and you should  preserve it.
				// Don't forget to declare your app's minimum required feature level in its
				// description.  All apps are assumed to support 9.1 unless otherwise stated.
				D3D_FEATURE_LEVEL featureLevels[] =
					{
						D3D_FEATURE_LEVEL_11_1,
						D3D_FEATURE_LEVEL_11_0,
						D3D_FEATURE_LEVEL_10_1,
						D3D_FEATURE_LEVEL_10_0,
						D3D_FEATURE_LEVEL_9_3,
						D3D_FEATURE_LEVEL_9_2,
						D3D_FEATURE_LEVEL_9_1
					};

				// Create the DX11 API device object, and get a corresponding context.

				//DX::ThrowIfFailed(
				//	D3D11CreateDevice(
				//		nullptr,                    // specify null to use the default adapter
				//		D3D_DRIVER_TYPE_HARDWARE,
				//		0,
				//		creationFlags,              // optionally set debug and Direct2D compatibility flags
				//		featureLevels,              // list of feature levels this app can support
				//		ARRAYSIZE(featureLevels),   // number of possible feature levels
				//		D3D11_SDK_VERSION,
				//		&d3d_device,                // returns the Direct3D device created
				//		&m_featureLevel,            // returns feature level of device created
				//		&d3d_context                // returns the device immediate context
				//	)
				//);
				//
				//// Obtain the underlying DXGI device of the Direct3D11 device.
				//DX::ThrowIfFailed(
				//	d3d_device.As(&dxgiDevice)
				//);
				//
				//// Obtain the Direct2D device for 2-D rendering.
				//DX::ThrowIfFailed(
				//	factory->CreateDevice(dxgiDevice.Get(), &d2d_device)
				//);
				//
				//// Get Direct2D device's corresponding device context object.
				//DX::ThrowIfFailed(
				//	d2d_device->CreateDeviceContext(
				//		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				//		&d2d_device_context
				//	)
				//);
				}

		private:
			details::wrappers::factory factory;
			Microsoft::WRL::ComPtr<ID3D11Device> d3d_device;
			Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
			Microsoft::WRL::ComPtr<ID2D1Device> d2d_device;

			Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d_context;
			Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d_device_context;
		};
	}//https://learn.microsoft.com/en-us/windows/win32/Direct2D/devices-and-device-contexts