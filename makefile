# i.e. https://itch-user.itch.io/itch-page
ITCH_USER = "itch-user"
ITCH_PAGE = "itch-page"

PROJECT_NAME 	= "Fishing Simulator 1692"
PROJECT_VERSION = "1.0"
BASE_WIDTH 		=  160
BASE_HEIGHT 	=  90

# Define PROJECT_NAME, PROJECT_VERSION, and resolution variables for use in code (I dont use them lol)
DEFINES = -DPROJECT_NAME=\"$(PROJECT_NAME)\" \
		  -DPROJECT_VERSION=\"$(PROJECT_VERSION)\" \
		  -DBASE_WIDTH=$(BASE_WIDTH) \
		  -DBASE_HEIGHT=$(BASE_HEIGHT)

# Search source folder and subfolders for .cpp files
CFILES = $(wildcard source/*.cpp source/*/*.cpp)

INCLUDES = -Iinclude/
LIBRARIES = ./libraylib.a

# Optimized for size | Warnings for all errors | Link with WASM Raylib | use shell.html | add assets folder to index.data | compatibility flags
WEB_OPT = -Os -Wall -Linclude/web $(LIBRARIES) --shell-file include/web/shell.html --preload-file assets -DPLATFORM_WEB -s USE_GLFW=3 -s GL_ENABLE_GET_PROC_ADDRESS
WEB_OUT = "build/index.html"
WEB_ZIP = "$(PROJECT_NAME)_v$(PROJECT_VERSION)_web.zip"

# Change this to your emscripten install location
# What I have is not recommended, i do it because cachyos probably makes mine faster
EMSDK_PATH = /lib/emscripten

# Use `emrun --list_browsers` for more options
BROWSER = /bin/chromium

# Build and run in browser
test_web: web_build web_run

# Build and zip for itch.io
release_web: web_build web_zip


web_build:
	mkdir -p build && \
	cd $(EMSDK_PATH) && cd "${PWD}" && \
	emcc -o $(WEB_OUT) $(DEFINES) $(CFILES) --preload-file ./resources $(INCLUDES) $(WEB_OPT)

web_run:
	cd $(EMSDK_PATH) && cd "${PWD}" && \
	emrun $(WEB_OUT) --browser $(BROWSER)

web_zip:
	cd build && zip $(WEB_ZIP) index.*

# Publish directly to itch.io (you will need to install and login to butler)
publish_web:
	butler push build/$(WEB_ZIP) $(ITCH_USER)/$(ITCH_PAGE):web --userversion $(PROJECT_VERSION)

# Delete generated files
clean:
	rm -rf build
