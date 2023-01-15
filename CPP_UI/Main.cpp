#include <utils/MS/window/window.h>
#include <utils/MS/window/style.h>
#include <utils/MS/window/regions.h>
#include <utils/MS/window/snap_on_drag.h>
#include <utils/MS/window/input/mouse.h>

#include "graphics/d2d/window.h"
#include "graphics/d2d/brush.h"

#include "UI/UI.h"

UI::core::element_own title_bar(utils::MS::window::base& window)
	{
	auto root{std::make_unique<UI::containers::stack>()};

	if (auto& drag_layer{root->emplace<UI::widgets::window_drag>(window)}) 
		{
		drag_layer.sizes.max_x = std::numeric_limits<float>::infinity();
		drag_layer.sizes.max_y = std::numeric_limits<float>::infinity();
		}
	if (auto& buttons_bar{root->emplace<UI::containers::group_hor>()})
		{
		buttons_bar.emplace<UI::widgets::spacer>();
		if (auto & button{buttons_bar.emplace<UI::widgets::button>([&window]() { window.minimize(); })})
			{
			button.sizes.max_x = 32;
			button.sizes.min_x = 32;
			}
		if (auto & button{buttons_bar.emplace<UI::widgets::button>([&window]() { window.maximize(); })})
			{
			button.sizes.max_x = 32;
			button.sizes.min_x = 32;
			}
		if (auto & button{buttons_bar.emplace<UI::widgets::button>([&window]() { window.close(); })})
			{
			button.sizes.max_x = 32;
			button.sizes.min_x = 32;
			}
		}

	root->sizes.max_y = 24;
	root->sizes.min_y = 24;

	return std::move(root);
	}

UI::core::element_own sample_ui(utils::MS::window::base& window)
	{
	auto root{std::make_unique<UI::containers::group_ver>()};
	root->alignment = UI::align_hor::left;

	root->push(title_bar(window));

	if (auto & top{root->emplace<UI::containers::group_hor>()})
		{
		top.alignment = UI::align_ver::top;
		top.sizes.max_y = 64;

		if (auto & padding{top.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 64;
				element.sizes.max_x = 128;
				//element->sizes.max.y =  64;
				}
			}

		if (auto & padding{top.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 32;
				element.sizes.max_x = 128;
				//element.sizes.max.y =  32;
				}
			}

		if (auto & padding{top.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 120;
				element.sizes.max_x = 128;
				}
			}

		if (auto & padding{top.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 120;
				element.sizes.max_x = 256;
				}
			}

		if (auto & padding{top.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 15;
				element.sizes.max_x = 64;
				}
			}
		}

	//generator<group_hor> root{};
	//auto&& bottom{root};
	if (auto & bottom{root->emplace<UI::containers::group_hor>()})
		{
		bottom.alignment = UI::align_ver::top;
		bottom.sizes.max_y = 128;

		if (auto & padding{bottom.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 64;
				element.sizes.max_x = 128;
				}
			}

		bottom.emplace<UI::widgets::spacer>();

		if (auto & padding{bottom.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 64;
				element.sizes.max_x = 128;
				}
			}
		}

	if (auto & sp{root->emplace<UI::widgets::spacer>()})
		{
		//sp.sizes.max_y = 32.f;
		}

	return std::move(root);
	}


std::vector<RAWINPUTDEVICELIST> GetRawInputDevices()
	{
	UINT deviceCount = 10; // initial guess, must be nonzero
	std::vector<RAWINPUTDEVICELIST> devices(deviceCount);
	while (deviceCount != 0) {
		UINT actualDeviceCount = GetRawInputDeviceList(
			devices.data(), &deviceCount,
			sizeof(devices[0]));
		if (actualDeviceCount != (UINT)-1) {
			devices.resize(actualDeviceCount);
			return devices;
			}
		DWORD error = GetLastError();
		if (error != ERROR_INSUFFICIENT_BUFFER) {
			std::terminate(); // throw something
			}
		devices.resize(deviceCount);
		}
	}

int main()
	{
	for(const auto& device : GetRawInputDevices())
		{
		switch (device.dwType)
			{
			case RIM_TYPEKEYBOARD: std::cout << "Keyboard "; break;
			case RIM_TYPEMOUSE   : std::cout << "Mouse    "; break;
			case RIM_TYPEHID     : std::cout << "HID      "; break;
			default:               std::cout << "Other    "; break;
			}
		std::cout << "(" << reinterpret_cast<uintptr_t>(device.hDevice) << ")" << std::endl;
		}



	// Setup rendering
	graphics::d2d::manager d2d_manager;

	utils::containers::object_pool<std::function<void(graphics::d2d::frame&)>> draw_operations;

	// Setup window 
	utils::MS::window::initializer window_initializer;
	
	utils::MS::window::base window
		{
		utils::MS::window::base::create_info
			{
			.title{L"George G. Goof"},
			//.position{{10, 60}},
			//.size{{800, 600}}
			},
		utils::MS::window::style::create_info
			{
			.transparency{utils::MS::window::style::transparency_t::composition_attribute},
			.borders     {utils::MS::window::style::value_t::disable}
			},
		utils::MS::window::resizable_edge::create_info
			{
			.thickness{8}
			},
		graphics::d2d::window::create_info
			{
			.manager{d2d_manager},
			.on_render{[&](graphics::d2d::window& windowd)
				{
				auto frame{windowd.begin_frame({.1f, .1f, .1f, 0.f})};
				frame.rt()->Clear(D2D1_COLOR_F{.r{0.f}, .g{0.f}, .b{1.f}, .a{.5f}});
				for (const auto& draw_operation : draw_operations)
					{
					draw_operation(frame);
					}
				}}
			},
		utils::MS::window::input::mouse::create_info{}
		};

	auto ui_root{sample_ui(window)};
	UI::window::create_info ui_create_info
		{
		.ui_root{ui_root},
		.mouse{window.get_module_ptr<utils::MS::window::input::mouse>()->default_mouse},
		.d2d_window_module{*window.get_module_ptr<graphics::d2d::window>()},
		};

	auto& window_ui{window.emplace_module_from_create_info(ui_create_info)};

	// Background grid
	draw_operations.emplace([&window](graphics::d2d::frame& frame)
		{
		// Draw a grid background.
		auto brush_handle{frame.rt().brushes_solid.create_solid(utils::graphics::colour::rgba{0, 1, 0})};
		
		int width  = static_cast<int>(window.client_rect.width ());
		int height = static_cast<int>(window.client_rect.height());
		
		for (int x = 0; x < width; x += 10)
			{
			frame.rt()->DrawLine
			(
				D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(0.0f  )),
				D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(height)),
				brush_handle->get(),
				0.5f
			);
			}
		
		for (int y = 0; y < height; y += 10)
			{
			frame.rt()->DrawLine
			(
				D2D1::Point2F(static_cast<FLOAT>(0.0f ), static_cast<FLOAT>(y)),
				D2D1::Point2F(static_cast<FLOAT>(width), static_cast<FLOAT>(y)),
				brush_handle->get(),
				0.5f
			);
			}
		});

	//Setup UI

	draw_operations.emplace([&window_ui](graphics::d2d::frame& frame)
		{
		window_ui.get_manager().debug_drawz();
		window_ui.get_manager().draw();
		});

	window.show();
	while (window.is_open())
		{
		window.wait_event();
		}
	}