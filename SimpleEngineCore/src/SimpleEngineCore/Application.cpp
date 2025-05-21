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
		m_window->set_event_callback(
			[](Event& event) {
				LOG_INFO("[Event] Changed size to {0}x{1}", event.width, event.height);
			}
		);
		while (true) {
			m_window->on_update();
			on_update();
		}

		return 0;
	}


}