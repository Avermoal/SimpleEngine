#ifndef SRC_SIMPLE_ENGINE_CORE_WINDOW_HPP
#define SRC_SIMPLE_ENGINE_CORE_WINDOW_HPP

#include <string>
#include <functional>

#include "SimpleEngineCore/Event.hpp"

struct GLFWwindow;

namespace SimpleEngine {
	class Window {
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;


		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator =(const Window&) = delete;
		Window& operator =(const Window&&) = delete;

		void on_update();

		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }

		void set_event_callback(const EventCallbackFn callback) {
			m_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData {
			std::string title;
			unsigned int width, height;
			EventCallbackFn eventCallbackFn;
		};



		int init();
		void shutdown();

		GLFWwindow* m_window = nullptr;
		WindowData m_data;
		float m_background_color[4] = { 1.f, 0.f, 0.f, 0.f };
	};
}

#endif//SRC_SIMPLE_ENGINE_CORE_WINDOW_HPP