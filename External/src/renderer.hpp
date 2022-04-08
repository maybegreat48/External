#pragma once
#include <imgui.h>
#include <dxgi.h>
#include <d3d11.h>
#include <Uxtheme.h>

namespace ext
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();

		void pre_reset();
		void post_reset();

		void on_tick();
		void toggle_opened();


		static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK keyproc(int nCode, WPARAM wparam, LPARAM lparam);
	public:
		ImFont* m_font;
		ImFont* m_font_title;
		ImFont* m_monospace_font;
	private:
		ID3D11DeviceContext* m_d3d_context{};
		ID3D11Device* m_device{};
		IDXGISwapChain* m_swapchain{};
		ID3D11RenderTargetView* m_render_view{};
		MARGINS m_margin = { -1 };
		MSG m_current_message = { NULL };
		HWND m_hwnd{};
		WNDCLASSEX m_win_class;
		std::thread m_renderer_thread;
		HHOOK m_keyboard_hook;
	};

	inline renderer* g_renderer{};
}