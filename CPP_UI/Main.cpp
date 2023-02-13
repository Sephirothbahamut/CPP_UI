#include <utils/MS/window/window.h>
#include <utils/MS/window/style.h>
#include <utils/MS/window/regions.h>
#include <utils/MS/window/snap_on_drag.h>
#include <utils/MS/window/input/mouse.h>

#include <utils/MS/graphics/d2d/window.h>

#include "UI/UI.h"

UI::core::element_own title_bar(utils::MS::window::base& window, utils::MS::graphics::dw::factory dw_factory, utils::MS::graphics::dw::text_format format, utils::MS::graphics::d2d::solid_brush brush, const std::wstring& string)
	{
	auto root{std::make_unique<UI::containers::stack>()};

	if (auto& drag_layer{root->emplace<UI::widgets::window_drag>(window)}) 
		{
		drag_layer.sizes.max_x = std::numeric_limits<float>::infinity();
		drag_layer.sizes.max_y = std::numeric_limits<float>::infinity();
		}
	if (auto& buttons_bar{root->emplace<UI::containers::group_hor>()})
		{
		buttons_bar.emplace<UI::drawables::text>(dw_factory, format, brush, string);
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

UI::core::element_own sample_ui(utils::MS::window::base& window, utils::MS::graphics::dw::factory dw_factory, utils::MS::graphics::dw::text_format format, utils::MS::graphics::d2d::solid_brush brush, const std::wstring& string)
	{
	auto root{std::make_unique<UI::containers::group_ver>()};
	root->alignment = UI::align_hor::left;

	root->push(title_bar(window, dw_factory, format, brush, string));

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
	utils::MS::graphics::co  ::initializer    co_initializer;
	utils::MS::graphics::d3d ::device         d3d_device;
	utils::MS::graphics::dxgi::device         dxgi_device{d3d_device};
	utils::MS::graphics::d2d ::factory        d2d_factory;
	utils::MS::graphics::d2d ::device         d2d_device{d2d_factory, dxgi_device};
	utils::MS::graphics::d2d ::device_context d2d_context{d2d_device};
	utils::MS::graphics::dw  ::factory        dw_factory;
	utils::MS::graphics::composition::device  composition_device{dxgi_device};
	UI::initializer ui_initializer{d2d_context};

	utils::containers::object_pool<std::function<void(const utils::MS::graphics::d2d::device_context&)>> draw_operations;

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
		//utils::MS::graphics::d2d::window::composition_swap_chain::create_info
		//	{
		//	.d2d_device{d2d_device},
		//	.on_render{[&](utils::MS::window::base& base, const utils::MS::graphics::d2d::device_context& context)
		//		{
		//		context->SetTransform(D2D1::IdentityMatrix());
		//		context->Clear(D2D1_COLOR_F{0.f, 0.f, 0.f, 0.f});
		//
		//		for (const auto& draw_operation : draw_operations)
		//			{
		//			draw_operation(context);
		//			}
		//		}}
		//	},
		utils::MS::graphics::d2d::window::render_target::create_info
			{
			.d2d_factory{d2d_factory},
			.on_render{[&](utils::MS::window::base& base, const utils::MS::graphics::d2d::device_context& context)
				{
				context->SetTransform(D2D1::IdentityMatrix());
				context->Clear(D2D1_COLOR_F{0.f, 0.f, 0.f, 0.f});
		
				for (const auto& draw_operation : draw_operations)
					{
					draw_operation(context);
					}
				}}
			},
		//utils::MS::graphics::d2d::window::swap_chain::create_info
		//	{
		//	.d2d_device{d2d_device},
		//	.on_render{[&](utils::MS::window::base& base, const utils::MS::graphics::d2d::device_context& context)
		//		{
		//		context->SetTransform(D2D1::IdentityMatrix());
		//		context->Clear(D2D1_COLOR_F{0.f, 0.f, 0.f, 0.f});
		//
		//		for (const auto& draw_operation : draw_operations)
		//			{
		//			draw_operation(context);
		//			}
		//		}}
		//	},
		utils::MS::window::input::mouse::create_info{}
		};

	auto& default_mouse{window.get_module_ptr<utils::MS::window::input::mouse>()->default_mouse};

	//UI resources
	utils::MS::graphics::dw::text_format font_format{dw_factory, utils::MS::graphics::dw::text_format::create_info
		{
		.name{L"Gabriola"},
		.size{16},
		.locale{L"en-gb"}
		}};
	utils::MS::graphics::d2d::solid_brush font_color{d2d_context, utils::graphics::colour::rgba_f{0.f, 0.f, 0.f, 1.f}};

	UI::manager ui_manager{ui_initializer, sample_ui(window, dw_factory, font_format, font_color, L"Donald Fauntleroy Duck"), default_mouse};

	auto& window_ui{window.emplace_module_from_create_info(UI::window::create_info{.manager_ptr{&ui_manager}})};

	// Background grid
	draw_operations.emplace([&window](const utils::MS::graphics::d2d::device_context& context)
		{
		// Draw a grid background.
		utils::MS::graphics::d2d::solid_brush brush{context, utils::graphics::colour::rgba_f{0.f, 1.f, 0.f, 1.f}};
		
		utils::math::vec2f sizef{static_cast<utils::math::vec2l>(window.client_rect.size())};
		
		for (float x = 0.f; x < sizef.x; x += 10.f)
			{
			context->DrawLine
				(
				D2D1::Point2F(x, 0.0f   ),
				D2D1::Point2F(x, sizef.y),
				brush.get(),
				0.5f
				);
			}
		for (float y = 0.f; y < sizef.y; y += 10.f)
			{
			context->DrawLine
				(
				D2D1::Point2F(0.0f   , y),
				D2D1::Point2F(sizef.x, y),
				brush.get(),
				0.5f
				);
			}
		});

	//Setup UI

	draw_operations.emplace([&ui_manager](const utils::MS::graphics::d2d::device_context& context)
		{
		ui_manager.debug_draw(context);
		ui_manager.draw(context);
		});

	window.show();
	while (window.is_open())
		{
		window.wait_event();
		}
	}