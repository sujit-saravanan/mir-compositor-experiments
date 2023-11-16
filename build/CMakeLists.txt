cmake_minimum_required(VERSION 3.16)
project(mir-compositor)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-I/usr/include/miral -I/usr/include/mircore -I/usr/include/freetype2 -lfreetype -lmiroil -lmircommon -lmircookie -lmirplatform -lmirserver -lmirwayland -lmiral -lwayland-egl -lwayland-client -lEGL")
set(CMAKE_EXPORT_COMPILE_COMMANDS )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)

set(VENDOR_HEADER_FILES
                ../vendor/glad/glad.h
                ../vendor/glad/khrplatform.h
                ../vendor/stb_image/stb_image.h
                )

set(INTERNAL_HEADER_FILES
                ../internal/io/io.h
                ../internal/texture/texture.h
                ../internal/render_pipelines/render_pipelines.h
                ../internal/buffer/buffer.h
                ../internal/spinner/spinner.h
                ../internal/miregl/miregl.h
                ../internal/eglapp/eglapp.h
                ../internal/wayland_app/wayland_app.h
                ../internal/wayland_surface/wayland_surface.h
                ../internal/floating_window_manager/floating_window_manager.h
                ../internal/splash_session.h/splash_session.h
                ../internal/decoration_provider/decoration_provider.h
                ../internal/wallpaper_config/wallpaper_config.h
                ../internal/wayland_shm/wayland_shm.h
                )

set(VENDOR_SOURCE_FILES
                ../vendor/glad/glad.c
                ../vendor/stb_image/stb_image.cpp
                )

set(INTERNAL_SOURCE_FILES
                ../internal/io/io.cpp
                ../internal/texture/texture.cpp
                ../internal/render_pipelines/render_pipelines.cpp
                ../internal/buffer/buffer.cpp
                ../internal/spinner/spinner.cpp
                ../internal/miregl/miregl.cpp
                ../internal/eglapp/eglapp.cpp
                ../internal/wayland_app/wayland_app.cpp
                ../internal/wayland_surface/wayland_surface.cpp
                ../internal/floating_window_manager/floating_window_manager.cpp
                ../internal/decoration_provider/decoration_provider.cpp
                ../internal/wallpaper_config/wallpaper_config.cpp
                ../internal/wayland_shm/wayland_shm.cpp
                )

set(SOURCE_FILES ../src/main.cpp
                ${INTERNAL_SOURCE_FILES}
                ${VENDOR_SOURCE_FILES}
                ${INTERNAL_HEADER_FILES}
                ${VENDOR_HEADER_FILES}
                )

include_directories(../vendor
                ../vendor/glad
                ../vendor/stb_image
                ../internal/io
                ../internal/texture
                ../internal/render_pipelines
                ../internal/buffer
                ../internal/spinner
                ../internal/miregl
                ../internal/eglapp
                ../internal/wayland_app
                ../internal/wayland_surface
                ../internal/floating_window_manager
                ../internal/splash_session.h
                ../internal/decoration_provider
                ../internal/wallpaper_config
                ../internal/wayland_shm
                )

add_executable(mir-compositor ${SOURCE_FILES})
target_precompile_headers(mir-compositor PRIVATE ${VENDOR_HEADER_FILES})
