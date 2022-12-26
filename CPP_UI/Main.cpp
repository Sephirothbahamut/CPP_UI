#include <utils/win32/window/window.h>
#include <utils/win32/window/style.h>
#include <utils/win32/window/regions.h>
#include <utils/win32/window/input_system.h>

#include "graphics/d2d/window.h"
#include "graphics/d2d/brush.h"

#include "UI/UI.h"

struct window_t : utils::win32::window::t<utils::win32::window::style, utils::win32::window::resizable_edge, utils::win32::window::input::mouse, graphics::d2d::window, UI::window>, utils::oop::devirtualize
	{
	struct create_info
		{
		utils::win32::window::base::create_info base;
		utils::win32::window::style::create_info style;
		graphics::d2d::window::create_info d2d;
		};

	window_t(create_info create_info) :
		utils::win32::window::base {create_info.base      },
		utils::win32::window::style{create_info.style     },
		graphics::d2d::window      {create_info.d2d       }
		{}
	};

UI::core::element_own title_bar(window_t& window)
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


UI::core::element_own sample_ui(window_t& window)
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
	window_t::initializer window_initializer;
	window_t window{window_t::create_info
		{
		.base
			{
			},
		.style
			{
			.transparency{utils::win32::window::style::transparency_t::composition_attribute},
			.borders{utils::win32::window::style::value_t::disable}
			},
		.d2d
			{
			.manager{d2d_manager},
			.on_render{[&](graphics::d2d::window& windowd)
				{
				auto frame{windowd.begin_frame({.1f, .1f, .1f, 0.f})};
				for (const auto& draw_operation : draw_operations)
					{
					draw_operation(frame);
					}
				}}
			}
		}};

	// Setup input
	utils::input::mouse mouse;
	window.mice_ptrs.emplace(&mouse);

	
	// Background grid
	draw_operations.emplace([&window](graphics::d2d::frame& frame)
		{
	
		// Draw a grid background.
		graphics::d2d::brush::solid_color brush{frame.rt(), utils::graphics::colour::rgba{0, 1, 0}};
	
		int width = static_cast<int>(window.client_rect.width);
		int height = static_cast<int>(window.client_rect.height);
	
		for (int x = 0; x < width; x += 10)
			{
			frame.rt()->DrawLine
				(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), height),
				brush.get(),
				0.5f
				);
			}
	
		for (int y = 0; y < height; y += 10)
			{
			frame.rt()->DrawLine
				(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(width, static_cast<FLOAT>(y)),
				brush.get(),
				0.5f
				);
			}
		});

	//Setup UI
	UI::manager ui_manager{sample_ui(window)};
	ui_manager.register_mouse_events(mouse);

	window.set_ui_manager(ui_manager);

	draw_operations.emplace([&ui_manager](graphics::d2d::frame& frame)
		{
		ui_manager.debug_draw(frame.rt());
		ui_manager.draw(frame.rt());
		});

	while (window.is_open()) { window.wait_event(); }
	}