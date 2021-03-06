#include <engine/window.hpp>

Window::Window (const char *title, const int width, const int height) {

  // #1 - Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "ERROR::WINDOW::SDL_INIT_ERROR\n";
    exit(EXIT_FAILURE);
  }

  //Use OpenGL 4.6 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // #2 - Create a Window and OpenGL Context
  window = SDL_CreateWindow(title,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width, height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "ERROR::WINDOW:SDL_CREATEWINDOW_ERROR\n";
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  context = SDL_GL_CreateContext(window);
  if (!context) {
    std::cout << "ERROR::WINDOW::SDL_GL_CONTEXT_ERROR\n";
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  // #3 - Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "ERROR::WINDOW::GLEWINIT_ERROR\n";
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  // TODO: what is the best thing to do for glViewport?
  glViewport(0, 0, width, height);

  // #4 - Enable Vsync (Optional)
  if (SDL_GL_SetSwapInterval(1) < 0) {
    std::cout << "ERROR::WINDOW::ENABLEVSYNC_ERROR\n";
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
}

Window::~Window () {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window::GraphicsAPI () {
  int major, minor, profile;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);

  std::cout << "OpenGL Version: " << major << '.' << minor;
  if (profile == SDL_GL_CONTEXT_PROFILE_CORE)
    std::cout << " | Profile: Core\n";
  else
    std::cout << " | Profile: Compatibility\n";
}

int Window::PollEvents () {
  return SDL_PollEvent(&event);
}

void Window::SwapBuffers () {
  SDL_GL_SwapWindow(window);
}

bool Window::shouldClose () {
  if (event.type == SDL_QUIT) return true;
  return false;
}
