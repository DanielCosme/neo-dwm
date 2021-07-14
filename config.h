/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "st"

/* appearance */
static unsigned int borderpx  = 0;        /* border pixel of windows */
static unsigned int snap      = 8;       /* snap pixel */
static unsigned int gappih    = 5;       /* horiz inner gap between windows */
static unsigned int gappiv    = 5;       /* vert inner gap between windows */
static unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=16";
static char normbgcolor[]           = "#282a36";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#0A0E14";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "110x40", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "alacritty", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,						XK_p,		spawn,			{.v = dmenucmd }    },
	{ MODKEY|ShiftMask,				XK_Return,	spawn,			{.v = termcmd } },
	{ MODKEY,						XK_b,		togglebar,		{0} },
	{ MODKEY,						XK_i,		incnmaster,     {.i = +1 } },
	{ MODKEY,						XK_d,		incnmaster,     {.i = -1 } },
	{ MODKEY,						XK_h,		setmfact,		{.f = -0.05} },
	{ MODKEY,						XK_l,		setmfact,      	{.f = +0.05} },
	{ MODKEY,						XK_Return,	zoom,			{0} }, 
	{ MODKEY,						XK_Tab,		view,			{0} },
	{ MODKEY,						XK_q,		killclient,		{0} },
	// layouts
	{ MODKEY,                       XK_space,	setlayout,      {0} }, // back to last layout
	{ MODKEY,						XK_t,		setlayout,		{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,				XK_t,		setlayout,		{.v = &layouts[1]} }, /* bstack */
	// { MODKEY,						XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	// { MODKEY|ShiftMask,				XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY|ShiftMask,				XK_m,		setlayout,		{.v = &layouts[4]} }, /* deck */
	{ MODKEY,						XK_m,		setlayout,		{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,						XK_y,		setlayout,		{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,				XK_y,		setlayout,		{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY|ShiftMask,				XK_f,		setlayout,		{.v = &layouts[8]} }, // floating
	{ MODKEY|ShiftMask,				XK_space,	togglefloating,	{0} },
	//{ MODKEY,						XK_0,		view,			{.ui = ~0 } },	// WTF??
	//{ MODKEY|ShiftMask,			XK_0,		tag,			{.ui = ~0 } },	// WTF??
	{ MODKEY,						XK_comma,	focusmon,		{.i = -1 } },
	{ MODKEY,						XK_period,	focusmon,		{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_comma,	tagmon,			{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_period,	tagmon,			{.i = +1 } },
	STACKKEYS(MODKEY,   			            focus)
	STACKKEYS(MODKEY|ShiftMask,					push)
	TAGKEYS(						XK_1,						0)
	TAGKEYS(						XK_2,						1)
	TAGKEYS(						XK_3,						2)
	TAGKEYS(						XK_4,						3)
	TAGKEYS(						XK_5,						4)
	TAGKEYS(						XK_6,						5)
	TAGKEYS(						XK_7,						6)
	TAGKEYS(						XK_8,						7)
	TAGKEYS(						XK_9,						8)
	{ MODKEY|ShiftMask,				XK_q,		quit,			{0} }, 
	// =============== New Stuff ===============
	// { MODKEY,						XK_semicolon,	shiftview,	{ .i = 1 } },// cycle tags
	//{ MODKEY|ShiftMask,				XK_semicolon,	shifttag,	{ .i = 1 } }, // ??
	// { MODKEY,						XK_apostrophe,	togglescratch,	{.ui = 1} }, ??
	{ MODKEY,						XK_0,		togglegaps,		{0} }, // maybe change
	{ MODKEY|ShiftMask,				XK_0,		defaultgaps,	{0} }, // maybe change
	{ MODKEY,						XK_equal,	incrgaps,		{.i = +3 } },
	{ MODKEY,						XK_minus,	incrgaps,		{.i = -3 } },
	{ MODKEY,						XK_s,		togglesticky,	{0} },
	{ MODKEY,						XK_f,		togglefullscr,	{0} },
	{ MODKEY|ShiftMask,				XK_u,		togglescratch,	{.ui = 0} },

	// FOR FUTURE REFERENCE.
	// { 0, XF86XK_AudioMute,					spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	// { 0, XF86XK_AudioRaiseVolume,			spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	// { 0, XF86XK_AudioLowerVolume,			spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	// { 0, XF86XK_AudioPrev,					spawn,		SHCMD("mpc prev") },
	// { 0, XF86XK_AudioNext,					spawn,		SHCMD("mpc next") },
	// { 0, XF86XK_AudioPause,					spawn,		SHCMD("mpc pause") },
	// { 0, XF86XK_AudioPlay,					spawn,		SHCMD("mpc play") },
	// { 0, XF86XK_AudioStop,					spawn,		SHCMD("mpc stop") },
	// { 0, XF86XK_AudioRewind,					spawn,		SHCMD("mpc seek -10") },
	// { 0, XF86XK_AudioForward,				spawn,		SHCMD("mpc seek +10") },
	// { 0, XF86XK_AudioMedia,					spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	// { 0, XF86XK_AudioMicMute,				spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	// { 0, XF86XK_PowerOff,					spawn,		SHCMD("sysact") },
	// { 0, XF86XK_Calculator,					spawn,		SHCMD(TERMINAL " -e bc -l") },
	// { 0, XF86XK_Sleep,						spawn,		SHCMD("sudo -A zzz") },
	// { 0, XF86XK_WWW,							spawn,		SHCMD("$BROWSER") },
	// { 0, XF86XK_DOS,							spawn,		SHCMD(TERMINAL) },
	// { 0, XF86XK_ScreenSaver,					spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	// { 0, XF86XK_TaskPane,					spawn,		SHCMD(TERMINAL " -e htop") },
	// { 0, XF86XK_Mail,						spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	// { 0, XF86XK_MyComputer,					spawn,		SHCMD(TERMINAL " -e lf /") },
	// /* { 0, XF86XK_Battery,					spawn,		SHCMD("") }, */
	// { 0, XF86XK_Launch1,						spawn,		SHCMD("xset dpms force off") },
	// { 0, XF86XK_TouchpadToggle,				spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	// { 0, XF86XK_TouchpadOff,					spawn,		SHCMD("synclient TouchpadOff=1") },
	// { 0, XF86XK_TouchpadOn,					spawn,		SHCMD("synclient TouchpadOff=0") },
	// { 0, XF86XK_MonBrightnessUp,				spawn,		SHCMD("xbacklight -inc 15") },
	// { 0, XF86XK_MonBrightnessDown,			spawn,		SHCMD("xbacklight -dec 15") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,			0,				Button4,		shiftview,	{.i = -1} },
	{ ClkTagBar,			0,				Button5,		shiftview,	{.i = 1} },
	{ ClkRootWin,			0,				Button2,		togglebar,	{0} },


	// MaYBE
	// { ClkClientWin,			MODKEY,			Button4,		incrgaps,		{.i = +1} },
	// { ClkClientWin,			MODKEY,			Button5,		incrgaps,		{.i = -1} },
};
