#include "SimpleEngineCore/Application.hpp"

#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Window.hpp"

namespace SimpleEngine {
	Application::Application() {
        LOG_INFO("Starting App");
	}
	Application::~Application() {
		LOG_INFO("Closing App");
	}

	int Application::start(unsigned int win_width, unsigned int win_height, const char* title) {
		m_window = std::make_unique<Window>(title, win_width, win_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event) {
				LOG_INFO("[Resize] Changed size to {0}x{1}", event.width, event.height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[Window close]");
				m_bCloseWindow = true;
			}
		);

		m_window->set_event_callback(
			[&](BaseEvent& event) {
				m_event_dispatcher.dispatch(event);
			}
		);

		while (!m_bCloseWindow) {
			m_window->on_update();
			on_update();
		}

		m_window = nullptr;

		return 0;
	}


}