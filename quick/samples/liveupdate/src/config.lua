RES_SERVER="https://raw.githubusercontent.com/katichar/Resource/master/"
GAME_VERSION = 0.01
-- 0 - disable debug info, 1 - less debug info, 2 - verbose debug info
DEBUG = 1

-- display FPS stats on screen
DEBUG_FPS = true

-- dump memory info every 10 seconds
DEBUG_MEM = false

-- load deprecated API
LOAD_DEPRECATED_API = false

-- load shortcodes API
LOAD_SHORTCODES_API = true

-- screen orientation
CONFIG_SCREEN_ORIENTATION = "landscape"

-- design resolution
CONFIG_SCREEN_WIDTH  = 800
CONFIG_SCREEN_HEIGHT = 480

-- auto scale mode
CONFIG_SCREEN_AUTOSCALE = "FIXED_HEIGHT"

local DESIGN_MAX_WIDTH  = 1136
local DESIGN_MAX_HEIGHT = 720
local DESIGN_MAX_RATIO  = DESIGN_MAX_WIDTH / DESIGN_MAX_HEIGHT

local DESIGN_MIN_WIDTH  = 960
local DESIGN_MIN_HEIGHT = 640
local DESIGN_MIN_RATIO  = DESIGN_MIN_WIDTH / DESIGN_MIN_HEIGHT

local DEVICE_SCREEN_WIDTH  = cc.Director:getInstance():getOpenGLView():getFrameSize().width
local DEVICE_SCREEN_HEIGHT = cc.Director:getInstance():getOpenGLView():getFrameSize().height
local DEVICE_SCREEN_RATIO  = DEVICE_SCREEN_WIDTH / DEVICE_SCREEN_HEIGHT

if DEVICE_SCREEN_RATIO < DESIGN_MAX_RATIO then
    CONFIG_SCREEN_WIDTH     = DESIGN_MAX_HEIGHT * DEVICE_SCREEN_RATIO
    CONFIG_SCREEN_HEIGHT    = DESIGN_MAX_HEIGHT
else
    CONFIG_SCREEN_WIDTH     = DESIGN_MAX_WIDTH
    CONFIG_SCREEN_HEIGHT    = DESIGN_MAX_WIDTH / DEVICE_SCREEN_RATIO
end