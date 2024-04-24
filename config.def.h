/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
#define BROWSER "firefox"
#define TERMINAL "footclient"

/* appearance */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static const unsigned int gappih           = 20; /* horiz inner gap between windows */
static const unsigned int gappiv           = 10; /* vert inner gap between windows */
static const unsigned int gappoh           = 10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov           = 30; /* vert outer gap between windows and screen edge */
static const int monoclegaps               = 0;  /* 1 means outer gaps in monocle layout */
static const int sloppyfocus               = 0;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartborders              = 1;
static const unsigned int borderpx         = 5;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x444444ff);
static const float focuscolor[]            = COLOR(0x005577ff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* To conform the xdg-protocol, set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */

static const unsigned int swipe_min_threshold = 0;

static const float default_opacity         = 1.0;

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
        "swaybg","-i", "/home/aboud/.config/ventilator-fan-blowing-appliance.jpg", NULL,
        "dunst",NULL,
        "firefox",NULL,
        "foot","--server",NULL,
        NULL /* terminate */
};


static const Rule rules[] = {
    /* app_id     title       tags mask     isfloating   alpha                isterm   noswallow  monitor   scratchkey */
   	{ NULL,     "scratchpad",   0,            1,         default_opacity,         1,      1,         -1,     's' },
   	{ NULL,     "filemanager",  0,            1,         default_opacity,         1,      1,         -1,     'f' },
   	{ NULL,     "calculator",   0,            1,         default_opacity,         1,      1,         -1,     'c' },
	{ "foot",   "foot",         0,            0,         default_opacity,         1,      1,         -1,       0 },
	{ "firefox",NULL,           8 << 1,       0,         default_opacity,         1,      1,         -1,       0 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "TTT",      bstack },
    { "===",      bstackhoriz },
};

/* monitors */
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = "caps:escape",
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;
/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "footclient" , NULL };
static const char *menucmd[] = { "bemenu-run", "--fb" ,"#1e1e2e" ,"--ff", "#cdd6f4" ,"--nb", "#1e1e2e" ,"--nf", "#cdd6f4", "--tb", "#1e1e2e", "--hb", "#1e1e2e", "--tf", "#f38ba8", "--hf", "#f9e2af" ,"--af", "#cdd6f4", "--ab" ,"#1e1e2e" ,NULL };
static const char *broswer[] = { "firefox", NULL };
/* named scratchpads - First arg only serves to match against key in rules*/
static const char *scratchpadcmd1[] = { "s", "footclient", "-T", "scratchpad","-w","700x500", NULL };
static const char *scratchpadcmd2[] = { "f", "footclient", "-T", "filemanager","-w","700x500","-e", "yazi", NULL };
static const char *scratchpadcmd3[] = { "c", "footclient", "-T", "calculator","-w","700x500","-e","python3", NULL };


#include "shiftview.c"
static const Gesture gestures[] = {
        { 0, SWIPE_LEFT, 4, shiftview, { .i = 1 } },
	{ 0, SWIPE_RIGHT, 4, shiftview, { .i = -1 } },
};
static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          movestack,      {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          movestack,      {.i = -1} },
	{ MODKEY,                    XKB_KEY_d,                    spawn,          {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_w,                    spawn,          {.v = broswer} },
	{ MODKEY,                    XKB_KEY_Return,               spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_z,          incgaps,       {.i = +3 } },
	{ MODKEY,                    XKB_KEY_x,          incgaps,       {.i = -3 } },
	{ MODKEY,                    XKB_KEY_a,          togglegaps,     {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT,XKB_KEY_A,           defaultgaps,    {0} },
    { MODKEY,        	XKB_KEY_e,		              spawn,            {.v = (const char*[]){ BROWSER,"gmail.com", NULL } } },
	{ MODKEY|WLR_MODIFIER_SHIFT,XKB_KEY_W,                    spawn,            {.v = (const char*[]){ BROWSER,"--private-window", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	XKB_KEY_d,                    spawn,            {.v = (const char*[]){ BROWSER,"datacamp.com", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	XKB_KEY_y,                    spawn,            {.v = (const char*[]){ BROWSER,"youtube.com", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	XKB_KEY_c,                    spawn,            {.v = (const char*[]){ BROWSER,"coursera.org", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	        XKB_KEY_l,                    spawn,            {.v = (const char*[]){ BROWSER,"lichess.org", NULL } } },
	{ MODKEY,                    XKB_KEY_semicolon,  shiftview,      { .i = 1 } },
	{ MODKEY|WLR_MODIFIER_ALT|WLR_MODIFIER_SHIFT,   XKB_KEY_L,        spawn,            {.v = (const char*[]){ BROWSER,"linkedin.com", NULL } } },
	{ MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_S,                    spawn,            {.v = (const char*[]){ BROWSER,"https://excalidraw.com/", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	XKB_KEY_w,                    spawn,            {.v = (const char*[]){ BROWSER,"web.whatsapp.com/", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT,	XKB_KEY_f,                    spawn,            {.v = (const char*[]){ BROWSER,"facebook.com", NULL } } },
  	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,      togglescratch,  {.v = scratchpadcmd1 } },
  	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_R,           togglescratch,  {.v = scratchpadcmd2 } },
  	{ MODKEY,                    XKB_KEY_c,           togglescratch,  {.v = scratchpadcmd3 } },
	{ 0,                         XKB_KEY_XF86AudioMute, spawn, {.v = (const char*[]){ "pactl","set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL } } },
	{ 0,                         XKB_KEY_XF86AudioRaiseVolume, spawn, {.v = (const char*[]){ "pactl","set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL } } },
	{ 0,                         XKB_KEY_XF86AudioLowerVolume, spawn, {.v = (const char*[]){ "pactl","set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL } } },
	{ MODKEY,			XKB_KEY_r,                    spawn,            {.v = (const char*[]){ TERMINAL, "-e", "yazi", NULL } } },
    { MODKEY,			XKB_KEY_m,		             spawn,            {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
    { MODKEY,			XKB_KEY_n,		             spawn,            {.v = (const char*[]){ TERMINAL,"-e", "nvim", NULL } } },
	{ MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_R,                    spawn,            {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
    { 0,                XKB_KEY_XF86Calculator,		 spawn,            {.v = (const char*[]){ TERMINAL, "-e", "python", NULL } } },
    { MODKEY,			XKB_KEY_period,		         spawn,            {.v = (const char*[]){  "obsidian", NULL} } },
    // Scripts
	{ MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_D,                    spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/passmenu", NULL } } },
	{ MODKEY,			            XKB_KEY_space,                spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/lang", NULL } } },
	{ MODKEY,			            XKB_KEY_s,                    spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/screenshot", NULL } } },
	{ MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_B,                    spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/bm.sh", NULL } } },
	{ MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_Q,                    spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/powermenu.sh", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT|WLR_MODIFIER_SHIFT,	XKB_KEY_Y,                spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/youtubesearch", NULL } } },
	{ MODKEY|WLR_MODIFIER_ALT|WLR_MODIFIER_SHIFT,	XKB_KEY_G,                spawn,            {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/googlesearch", NULL } } },
	{ MODKEY,			XKB_KEY_Insert,	             spawn,            SHCMD("ydotool type $(grep -v '^#' /home/aboud/.local/bin/scripts/dmenu/bookmarks | bemenu --fb \"#1e1e2e\" --ff \"#cdd6f4\" --nb \"#1e1e2e\" --nf \"#cdd6f4\" --tb \"#1e1e2e\" --hb \"#1e1e2e\" --tf \"#f38ba8\" --hf \"#f9e2af\" --af \"#cdd6f4\" --ab \"#1e1e2e\" -l 10 | cut -d' ' -f1)") },
    { MODKEY|WLR_MODIFIER_SHIFT,	XKB_KEY_E,		             spawn,            SHCMD("ydotool type $(grep -v '^#' /home/aboud/.local/bin/scripts/dmenu/emails | bemenu --fb \"#1e1e2e\" --ff \"#cdd6f4\" --nb \"#1e1e2e\" --nf \"#cdd6f4\" --tb \"#1e1e2e\" --hb \"#1e1e2e\" --tf \"#f38ba8\" --hf \"#f9e2af\" --af \"#cdd6f4\" --ab \"#1e1e2e\" -l 10 | cut -d' ' -f1)") },
    { 0,                         XKB_KEY_XF86MonBrightnessUp,  spawn, SHCMD("light -A 5") },
	{ 0,                         XKB_KEY_XF86MonBrightnessDown,spawn, SHCMD("light -U 5")  },
	{ MODKEY,                    XKB_KEY_b,spawn, SHCMD("somebar -c toggle all")  },
	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
   	{ MODKEY,                    XKB_KEY_o,          setopacity,     {.f = +0.1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_O,          setopacity,     {.f = -0.1} },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_u,          setlayout,      {.v = &layouts[3]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_U,          setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                    XKB_KEY_i,      setlayout,      {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_f,         togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,            {.ui = ~0} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                  8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,          spawn, {.v = (const char*[]){ "/home/aboud/.local/bin/scripts/dmenu/powermenu.sh", NULL } } },
	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to {.v = (const char*[]){ "light -U 5", NULL } }be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
