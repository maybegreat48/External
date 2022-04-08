#include "renderer.hpp"
#include "gui.hpp"
#include "fonts/fonts.hpp"

#include <dwmapi.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <imgui_internal.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ext
{

	renderer::renderer() :
		m_win_class({sizeof(WNDCLASSEX), 0, wndproc, 0, 0, nullptr, ::LoadIcon(nullptr, IDI_SHIELD), ::LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, L"renderer", ::LoadIcon(nullptr, IDI_SHIELD)})
	{
		auto x = ::GetSystemMetrics(SM_CXSCREEN);
		auto y = ::GetSystemMetrics(SM_CYSCREEN);

		::RegisterClassEx(&m_win_class);
		m_hwnd = ::CreateWindowEx(NULL, L"renderer", L"renderer", WS_POPUP, 0, 0, x, y, NULL, NULL, m_win_class.hInstance, NULL);
		//if (!m_hwnd)
		//	LOG(FATAL) << GetLastError();

		::SetLayeredWindowAttributes(m_hwnd, 0, 0, LWA_ALPHA);
		::SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
		::DwmExtendFrameIntoClientArea(m_hwnd, &m_margin);
		::SetWindowLong(m_hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
		::ShowWindow(m_hwnd, SW_SHOW);
		::UpdateWindow(m_hwnd);
		::SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		::memset(&swapchain_desc, 0, sizeof(swapchain_desc));
		swapchain_desc.BufferCount = 2;
		swapchain_desc.BufferDesc.Width = 0;
		swapchain_desc.BufferDesc.Height = 0;
		swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.OutputWindow = m_hwnd;
		swapchain_desc.SampleDesc.Count = 1;
		swapchain_desc.SampleDesc.Quality = 0;
		swapchain_desc.Windowed = TRUE;
		swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		D3D_FEATURE_LEVEL vFeatureLevel;
		D3D_FEATURE_LEVEL vFeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		if (::D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, vFeatureLevelArray, 2, D3D11_SDK_VERSION, &swapchain_desc, &m_swapchain, &m_device, &vFeatureLevel, &m_d3d_context) != S_OK)
			throw std::exception("failed to create device");

		ID3D11Texture2D* backbuf = nullptr;
		m_swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuf));
		m_device->CreateRenderTargetView(backbuf, NULL, &m_render_view);
		backbuf->Release();

		IMGUI_CHECKVERSION();
		auto ctx = ImGui::CreateContext();

		auto file_path = g_file_manager->get_project_file("./imgui.ini").get_path();
		static std::string path = file_path.make_preferred().string();
		ctx->IO.IniFilename = path.c_str();

		ImGui_ImplWin32_Init(m_hwnd);
		ImGui_ImplDX11_Init(m_device, m_d3d_context);

		m_d3d_context->Release();

		ImFontConfig font_cfg{};
		font_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_cfg.Name, "Storopia");

		m_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_storopia), sizeof(font_storopia), 20.f, &font_cfg);
		m_monospace_font = ImGui::GetIO().Fonts->AddFontDefault();

		g->window.font_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_storopia), sizeof(font_storopia), 40.f, &font_cfg);
		g->window.font_sub_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_storopia), sizeof(font_storopia), 30.f, &font_cfg);
		g->window.font_small = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_storopia), sizeof(font_storopia), 17.5f, &font_cfg);

		m_keyboard_hook = ::SetWindowsHookEx(WH_KEYBOARD_LL, &renderer::keyproc, nullptr, 0);

		g_gui.dx_init();

		g_renderer = this;
		//m_renderer_thread = std::thread(on_tick);
		//m_renderer_thread.detach();
	}

	renderer::~renderer()
	{
		::UnregisterClass(L"renderer", m_win_class.hInstance);
		::UnhookWindowsHookEx(m_keyboard_hook);
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		::DestroyWindow(m_hwnd);

		g_renderer = nullptr;
	}

	void renderer::on_present()
	{
		if (g_gui.m_opened)
		{
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			ImGui::GetIO().MouseDrawCursor = true;
		}
		else
		{
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			ImGui::GetIO().MouseDrawCursor = false;
		}

		g_gui.always_draw();

		if (g_gui.m_opened)
		{
			g_gui.dx_on_tick();
		}
	}

	void renderer::pre_reset()
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();

		if (m_render_view)
		{
			m_render_view->Release();
			m_render_view = nullptr;
		}
	}

	void renderer::post_reset()
	{
		ImGui_ImplDX11_CreateDeviceObjects();

		ID3D11Texture2D* backbuf = nullptr;
		m_swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuf));
		m_device->CreateRenderTargetView(backbuf, NULL, &m_render_view);
		backbuf->Release();
	}

	LRESULT CALLBACK renderer::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_renderer) g_renderer->m_hwnd = hwnd;
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;

		switch (msg) 
		{
		case WM_SIZE:
			if (g_renderer && g_renderer->m_device != NULL && wparam != SIZE_MINIMIZED) {
				g_renderer->pre_reset();
				g_renderer->m_swapchain->ResizeBuffers(0, LOWORD(lparam), HIWORD(lparam), DXGI_FORMAT_UNKNOWN, 0);
				g_renderer->post_reset();
			}
			break;
		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
			break;
		}
		return 0;
	}

	LRESULT CALLBACK renderer::keyproc(int nCode, WPARAM wparam, LPARAM lparam)
	{
		if (nCode == HC_ACTION)
		{
			KBDLLHOOKSTRUCT* hookstruct = (KBDLLHOOKSTRUCT*)lparam;
			ImGui_ImplWin32_WndProcHandler(g_renderer->m_hwnd, wparam, hookstruct->vkCode, 0);
		}

		return ::CallNextHookEx(nullptr, nCode, wparam, lparam);
	}

	void renderer::on_tick()
	{
		std::memset(&m_current_message, 0, sizeof(MSG));

		while (m_current_message.message != WM_QUIT && g_running) 
		{
			if (::PeekMessage(&m_current_message, m_hwnd, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&m_current_message);
				::DispatchMessage(&m_current_message);
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			on_present(); // inaccurate but it works

			ImGui::EndFrame();
			ImGui::Render();

			static float color[4]{};

			m_d3d_context->OMSetRenderTargets(1, &m_render_view, nullptr);
			m_d3d_context->ClearRenderTargetView(m_render_view, color);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			m_swapchain->Present(1, 0);

			::Sleep(0);
		}
	}

	void renderer::toggle_opened()
	{
		g_gui.m_opened = !g_gui.m_opened;
		DWORD flags;
		::GetLayeredWindowAttributes(m_hwnd, 0, 0, &flags);

		if (g_gui.m_opened)
		{
			flags &= ~WS_EX_LAYERED;
		}
		else
		{
			flags |= WS_EX_LAYERED;
		}

		::SetLayeredWindowAttributes(m_hwnd, 0, 0, flags);
	}
}