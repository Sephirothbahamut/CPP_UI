#include <thread>

#include <utils/MS/window/window.h>
#include <utils/MS/window/style.h>
#include <utils/MS/window/regions.h>
#include <utils/MS/window/snap_on_drag.h>
#include <utils/MS/window/input/mouse.h>

#include <utils/MS/graphics/d2d/window.h>

#include "UI/UI.h"

struct style
	{
	utils::MS::graphics::dw::factory dw_factory;

	struct text_t
		{
		utils::MS::graphics::dw::text_format format;
		utils::MS::graphics::d2d::brush      brush;
		} text;

	struct button_t
		{
		UI::drawables::draw_shape_data normal;
		UI::drawables::draw_shape_data hover;
		UI::drawables::draw_shape_data down;
		text_t text;
		} button;
	};

std::unique_ptr<UI::widgets::button> make_button(const style& style, const std::wstring& text, std::function<void()> callback)
	{
	auto make_layer{[&](const UI::drawables::draw_shape_data& draw_shape_data)
		{
		auto root{std::make_unique<UI::containers::overlay>()};
		root->align_hor = UI::core::align_hor::center;
		root->align_ver = UI::core::align_ver::middle;

		if (auto & rect{root->emplace<UI::drawables::rect>()})
			{
			rect.draw_shape_data = draw_shape_data;
			}
		root->emplace<UI::drawables::text>(style.dw_factory, style.button.text.format, style.button.text.brush, text);
		return root;
		}};

	auto root{std::make_unique<UI::widgets::button>(callback)};
	root->align_hor = UI::core::align_hor::center;
	root->align_ver = UI::core::align_ver::middle;
	root->sizes.min_x = 128.f;
	root->sizes.min_y =  24.f;
	root->sizes.prf_x = 128.f;
	root->sizes.prf_y =  24.f;
	root->sizes.max_x = 256.f;
	root->sizes.max_y =  32.f;

	root->push(make_layer(style.button.normal));
	root->push(make_layer(style.button.hover ));
	root->push(make_layer(style.button.down  ));

	return root;
	}

UI::core::element_own title_bar(utils::MS::window::base& window, const style& style, const std::wstring& string)
	{
	auto root{std::make_unique<UI::containers::overlay>()};
	root->sizes.max_y = 24;
	root->sizes.min_y = 24;

	if (auto& drag_layer{root->emplace<UI::widgets::window_drag>(window)}) 
		{
		drag_layer.sizes.max_x = std::numeric_limits<float>::infinity();
		drag_layer.sizes.max_y = std::numeric_limits<float>::infinity();
		}

	if (auto& buttons_bar{root->emplace<UI::containers::group_hor>()})
		{
		buttons_bar.alignment = UI::core::align_ver::middle;
		if (auto& text{buttons_bar.emplace<UI::drawables::text>(style.dw_factory, style.text.format, style.text.brush, string)})
			{
			text.sizes.min_x = 0.f;
			text.sizes.prf_x = 1;
			text.sizes.max_x = UI::core::finf;
			}

		//buttons_bar.emplace<UI::widgets::spacer>();
		if (auto btn{make_button(style, L"–", [&window]() {window.minimize(); })})
			{
			btn->sizes.min_x = 32.f;
			btn->sizes.prf_x = 32.f;
			btn->sizes.max_x = 32.f;
			buttons_bar.push(std::move(btn));
			}
		if (auto btn{make_button(style, L"🗔", [&window]() { if (window.is_maximized()) { window.restore(); } else { window.maximize(); } })})
			{
			btn->sizes.min_x = 32.f;
			btn->sizes.prf_x = 32.f;
			btn->sizes.max_x = 32.f;
			buttons_bar.push(std::move(btn));
			}
		if (auto btn{make_button(style, L"❌", [&window]() {window.close(); })})
			{
			btn->sizes.min_x = 32.f;
			btn->sizes.prf_x = 32.f;
			btn->sizes.max_x = 32.f;
			buttons_bar.push(std::move(btn));
			}
		}

	return std::move(root);
	}

UI::core::element_own sample_ui(utils::MS::window::base& window, const style& style, const std::wstring& string)
	{
	auto root{std::make_unique<UI::containers::group_ver>()};
	root->alignment = UI::align_hor::left;

	root->push(title_bar(window, style, string));

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

	//auto root{std::make_unique<UI::containers::group_hor>()};
	//root->alignment = UI::align_ver::top;
	//auto& bottom{*root};
	if (auto & bottom{root->emplace<UI::containers::group_hor>()})
		{
		bottom.alignment = UI::align_ver::top;
		bottom.sizes.max_y = 128;

		if (auto & padding{bottom.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 128;
				element.sizes.max_x = 256;
				}
			}

		bottom.emplace<UI::widgets::spacer>();

		if (auto & padding{bottom.emplace<UI::wrappers::padding>(utils::math::geometry::aabb{.ll{8}, .up{8}, .rr{8}, .dw{8}})})
			{
			if (auto & element{padding.emplace<UI::widgets::dummy>()})
				{
				element.sizes.min_x = 10;
				element.sizes.prf_x = 128;
				element.sizes.max_x = 256;
				}
			}
		}

	//if (auto & body{root->emplace<UI::containers::group_ver>()})
		{
		auto& body{*root};
		body.push(make_button(style, L"Hello world!", []() {std::cout << "Hello world!" << std::endl; }));
		body.push(make_button(style, L"Hello cats! ", []() {std::cout << "Hello cats! " << std::endl; }));
		body.push(make_button(style, L"Hello dogs! ", []() {std::cout << "Hello dogs! " << std::endl; }));
		}

	root->emplace<UI::widgets::spacer>();

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
	return devices;
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

	utils::MS::window::initializer window_initializer;

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
	//UI resources

	utils::MS::window::base window_a{utils::MS::window::base::create_info{.title{L"George G. Goof"}}};
	utils::MS::window::base window_b{utils::MS::window::base::create_info{.title{L"George G. Goof"}}};
	
	utils::MS::graphics::d2d::device_context a_d2d_device_context{d2d_device};
	utils::MS::graphics::composition::device a_composition_device{d2d_device.get_dxgi_device()};
	utils::MS::graphics::composition::visual a_composition_visual{composition_device};
	utils::MS::graphics::composition::target a_composition_target{composition_device, window_a.get_handle()};
	utils::MS::graphics::dxgi::swap_chain    a_dxgi_swapchain{d2d_device.get_dxgi_device(), window_a.get_handle(), nullptr};
	
	utils::MS::graphics::d2d::device_context b_d2d_device_context{d2d_device};
	utils::MS::graphics::composition::device b_composition_device{d2d_device.get_dxgi_device()};
	utils::MS::graphics::composition::visual b_composition_visual{composition_device};
	utils::MS::graphics::composition::target b_composition_target{composition_device, window_b.get_handle()};
	utils::MS::graphics::dxgi::swap_chain    b_dxgi_swapchain{d2d_device.get_dxgi_device(), window_b.get_handle(), nullptr};

	style style
		{
		.dw_factory{dw_factory},
		.text
			{
			.format
				{
				dw_factory,
				utils::MS::graphics::dw::text_format::create_info
					{
					.name{L"Arial"},
					//.name{L"Gabriola"},
					.size{16},
					.alignment_ver{utils::MS::graphics::dw::text_format::alignment_ver::center},
					.locale{L"en-gb"},
					}
				},
			.brush{utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{0.9f, 0.9f, 0.9f, 1.f}}.as<utils::MS::graphics::d2d::brush>()}
			},
		.button
			{
			.normal
				{
				.brush_fill_opt   {utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.6f, 0.6f, 0.6f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.brush_outline_opt{utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.5f, 0.5f, 0.5f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.outline_thickness{2.f}
				},
			.hover
				{
				.brush_fill_opt   {utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.7f, 0.7f, 0.7f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.brush_outline_opt{utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.6f, 0.6f, 0.6f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.outline_thickness{2.f}
				},
			.down
				{
				.brush_fill_opt   {utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.3f, 0.3f, 0.3f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.brush_outline_opt{utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{.4f, 0.4f, 0.4f, 1.f}}.as<utils::MS::graphics::d2d::brush>()},
				.outline_thickness{2.f}
				},
			.text
				{
				.format
					{
					dw_factory,
					utils::MS::graphics::dw::text_format::create_info
						{
						.name{L"Arial"},
						//.name{L"Gabriola"},
						.size{16},
						.alignment_hor{utils::MS::graphics::dw::text_format::alignment_hor::center},
						.alignment_ver{utils::MS::graphics::dw::text_format::alignment_ver::center},
						.locale{L"en-gb"},
						}
					},
				.brush{utils::MS::graphics::d2d::solid_brush{d2d_context, utils::graphics::colour::rgba_f{0.9f, 0.9f, 0.9f, 1.f}}.as<utils::MS::graphics::d2d::brush>()}
				}
			}
		};

	DWRITE_TRIMMING trimming
		{
		.granularity   {DWRITE_TRIMMING_GRANULARITY_CHARACTER},
		//.delimiter     {static_cast<uint32_t>(L'…')}, //0x2026
		//.delimiterCount{1}
		//.delimiter     {static_cast<uint32_t>(L'.')}, //0x00B7
		//.delimiterCount{3}
		.delimiter     {0x00B7},
		.delimiterCount{3}
		};
	style.text.format->SetTrimming(&trimming, nullptr);

	auto spawn_window{[&]()
		{
		utils::containers::object_pool<std::function<void(const utils::MS::graphics::d2d::device_context&)>> draw_operations;

		// Setup window 
	
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
			//utils::MS::graphics::d2d::window::render_target::create_info
			//utils::MS::graphics::d2d::window::swap_chain::create_info
			utils::MS::graphics::d2d::window::composition_swap_chain::create_info
				{
				//.d2d_factory{d2d_factory},
				.d2d_device{d2d_device},
				.draw_callback{[&](const utils::MS::window::base& base, const utils::MS::graphics::d2d::device_context& context)
					{
					context->SetTransform(D2D1::IdentityMatrix());
					context->Clear(D2D1_COLOR_F{0.f, 0.f, 0.f, 0.f});
			
					for (const auto& draw_operation : draw_operations)
						{
						draw_operation(context);
						}
					}}
				},
			utils::MS::window::input::mouse::create_info{}
			};

		// Background grid
		draw_operations.emplace([&window](const utils::MS::graphics::d2d::device_context& context)
			{
			// Draw a grid background.
			utils::MS::graphics::d2d::solid_brush brush{context, utils::graphics::colour::rgba_f{0.f, 1.f, 0.f, 0.1f}};
		
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

		auto& default_mouse{window.get_module_ptr<utils::MS::window::input::mouse>()->default_mouse};
		
		UI::manager ui_manager{ui_initializer, sample_ui(window, style, L"Donald Fauntleroy Duck"), default_mouse};
		
		auto& window_ui{window.emplace_module_from_create_info(UI::window::create_info{.manager_ptr{&ui_manager}})};
		
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
		}};

	std::vector<std::jthread> threads;
	threads.emplace_back([&spawn_window]()
		{
		try
			{
			spawn_window();
			}
		catch (const std::exception& e) { std::cout << e.what() << std::endl; }
		});
	threads.emplace_back([&spawn_window]()
		{
		try
			{
			spawn_window();
			}
		catch (const std::exception& e) { std::cout << e.what() << std::endl; }
		});
	}