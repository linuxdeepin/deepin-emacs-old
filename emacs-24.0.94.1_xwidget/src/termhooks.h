/* Parameters and display hooks for terminal devices.

Copyright (C) 1985-1986, 1993-1994, 2001-2012  Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.  */


/* Miscellanea.   */

#include "systime.h" /* for Time */

struct glyph;
struct frame;


enum scroll_bar_part {
  scroll_bar_above_handle,
  scroll_bar_handle,
  scroll_bar_below_handle,
  scroll_bar_up_arrow,
  scroll_bar_down_arrow,
  scroll_bar_to_top,
  scroll_bar_to_bottom,
  scroll_bar_end_scroll,
  scroll_bar_move_ratio
};

/* If the value of the frame parameter changed, whis hook is called.
   For example, if going from fullscreen to not fullscreen this hook
   may do something OS dependent, like extended window manager hints on X11.  */
extern void (*fullscreen_hook) (struct frame *f);


/* Input queue declarations and hooks.  */

enum event_kind
{
  NO_EVENT,			/* nothing happened.  This should never
				   actually appear in the event queue.  */

  ASCII_KEYSTROKE_EVENT,	/* The ASCII code is in .code, perhaps
				   with modifiers applied.
				   .modifiers holds the state of the
				   modifier keys.
				   .frame_or_window is the frame in
				   which the key was typed.
				   .timestamp gives a timestamp (in
				   milliseconds) for the keystroke.  */
  MULTIBYTE_CHAR_KEYSTROKE_EVENT,	/* The multibyte char code is in .code,
				   perhaps with modifiers applied.
				   The others are the same as
				   ASCII_KEYSTROKE_EVENT.  */
  NON_ASCII_KEYSTROKE_EVENT,	/* .code is a number identifying the
				   function key.  A code N represents
				   a key whose name is
				   function_key_names[N]; function_key_names
				   is a table in keyboard.c to which you
				   should feel free to add missing keys.
				   .modifiers holds the state of the
				   modifier keys.
				   .frame_or_window is the frame in
				   which the key was typed.
				   .timestamp gives a timestamp (in
				   milliseconds) for the keystroke.  */
  TIMER_EVENT,                  /* A timer fired.  */
  MOUSE_CLICK_EVENT,		/* The button number is in .code; it must
				   be >= 0 and < NUM_MOUSE_BUTTONS, defined
				   below.
				   .modifiers holds the state of the
				   modifier keys.
				   .x and .y give the mouse position,
				   in characters, within the window.
				   .frame_or_window gives the frame
				   the mouse click occurred in.
				   .timestamp gives a timestamp (in
				   milliseconds) for the click.  */
  WHEEL_EVENT,			/* A wheel event is generated by a
				   wheel on a mouse (e.g., MS
				   Intellimouse).
				   .modifiers holds the rotate
				   direction (up or down), and the
				   state of the modifier keys.
				   .x and .y give the mouse position,
				   in characters, within the window.
				   .frame_or_window gives the frame
				   the wheel event occurred in.
				   .timestamp gives a timestamp (in
				   milliseconds) for the event.  */
  HORIZ_WHEEL_EVENT,            /* A wheel event generated by a second
                                   horizontal wheel that is present on some
                                   mice. See WHEEL_EVENT.  */
#if defined (WINDOWSNT)
  LANGUAGE_CHANGE_EVENT,	/* A LANGUAGE_CHANGE_EVENT is
				   generated on WINDOWSNT or Mac OS
				   when the keyboard layout or input
				   language is changed by the
				   user.  */
#endif
  SCROLL_BAR_CLICK_EVENT,	/* .code gives the number of the mouse button
				   that was clicked.
				   .modifiers holds the state of the modifier
				   keys.
				   .part is a lisp symbol indicating which
				   part of the scroll bar got clicked.
				   .x gives the distance from the start of the
				   scroll bar of the click; .y gives the total
				   length of the scroll bar.
				   .frame_or_window gives the window
				   whose scroll bar was clicked in.
				   .timestamp gives a timestamp (in
				   milliseconds) for the click.  */
  SELECTION_REQUEST_EVENT,	/* Another X client wants a selection from us.
				   See `struct selection_input_event'.  */
  SELECTION_CLEAR_EVENT,	/* Another X client cleared our selection.  */
  BUFFER_SWITCH_EVENT,		/* A process filter has switched buffers.  */
  DELETE_WINDOW_EVENT,		/* An X client said "delete this window".  */
  MENU_BAR_EVENT,		/* An event generated by the menu bar.
				   The frame_or_window field's cdr holds the
				   Lisp-level event value.
				   (Only the toolkit version uses these.)  */
  ICONIFY_EVENT,		/* An X client iconified this window.  */
  DEICONIFY_EVENT,		/* An X client deiconified this window.  */
  MENU_BAR_ACTIVATE_EVENT,      /* A button press in the menu bar
				   (toolkit version only).  */
  DRAG_N_DROP_EVENT,		/* A drag-n-drop event is generated when
				   files selected outside of Emacs are dropped
				   onto an Emacs window.
				   .modifiers holds the state of the
				   modifier keys.
				   .x and .y give the mouse position,
				   in characters, within the window.
				   .frame_or_window is the frame in
				   which the drop was made.
				   .arg is a platform-dependent
				   representation of the dropped items.
				   .timestamp gives a timestamp (in
				   milliseconds) for the click.  */
  USER_SIGNAL_EVENT,		/* A user signal.
                                   code is a number identifying it,
                                   index into lispy_user_signals.  */

  /* Help events.  Member `frame_or_window' of the input_event is the
     frame on which the event occurred, and member `arg' contains
     the help to show.  */
  HELP_EVENT,

  /* An event from a tool-bar.  Member `arg' of the input event
     contains the tool-bar item selected.  If `frame_or_window'
     and `arg' are equal, this is a prefix event.  */
  TOOL_BAR_EVENT,

  /* Queued from XTread_socket on FocusIn events.  Translated into
     `switch-frame' events in kbd_buffer_get_event, if necessary.  */
  FOCUS_IN_EVENT,

  /* Generated when mouse moves over window not currently selected.  */
  SELECT_WINDOW_EVENT,

  /* Queued from XTread_socket when session manager sends
     save yourself before shutdown. */
  SAVE_SESSION_EVENT

#ifdef HAVE_GPM
  , GPM_CLICK_EVENT
#endif

#ifdef HAVE_DBUS
  , DBUS_EVENT
#endif

  , CONFIG_CHANGED_EVENT

#ifdef WINDOWSNT
  /* Generated when an APPCOMMAND event is received, in response to
     Multimedia or Internet buttons on some keyboards.
     Such keys are available as normal function keys on X through the
     Xkeyboard extension.
     On Windows, some of them get mapped to normal function key events,
     but others need to be handled by APPCOMMAND. Handling them all as
     APPCOMMAND events means they can be disabled
     (w32-pass-multimedia-buttons-to-system), important on Windows since
     the system never sees these keys if Emacs claims to handle them.
     On X, the window manager seems to grab the keys it wants
     first, so this is not a problem there.  */
  , MULTIMEDIA_KEY_EVENT
#endif

#ifdef HAVE_NS
  /* Generated when native multi-keystroke input method is used to modify
     tentative or indicative text display. */
  , NS_TEXT_EVENT
  /* Non-key system events (e.g. application menu events) */
  , NS_NONKEY_EVENT
#endif
#ifdef HAVE_XWIDGETS
  /* events generated by xwidgets*/
   , XWIDGET_EVENT
#endif
};

/* If a struct input_event has a kind which is SELECTION_REQUEST_EVENT
   or SELECTION_CLEAR_EVENT, then its contents are really described
   by `struct selection_input_event'; see xterm.h.  */

/* The keyboard input buffer is an array of these structures.  Each one
   represents some sort of input event - a keystroke, a mouse click, or
   a window system event.  These get turned into their lispy forms when
   they are removed from the event queue.  */

struct input_event
{
  /* What kind of event was this?  */
  enum event_kind kind;

  /* For an ASCII_KEYSTROKE_EVENT and MULTIBYTE_CHAR_KEYSTROKE_EVENT,
     this is the character.
     For a NON_ASCII_KEYSTROKE_EVENT, this is the keysym code.
     For a mouse event, this is the button number.
     For a HELP_EVENT, this is the position within the object
      (stored in ARG below) where the help was found.  */
  /* In WindowsNT, for a mouse wheel event, this is the delta.  */
  EMACS_INT code;
  enum scroll_bar_part part;

  int modifiers;		/* See enum below for interpretation.  */

  Lisp_Object x, y;
  Time timestamp;

  /* This is padding just to put the frame_or_window field
     past the size of struct selection_input_event.  */
  int *padding[2];

  /* This field is copied into a vector while the event is in the queue,
     so that garbage collections won't kill it.  */
  /* In a menu_bar_event, this is a cons cell whose car is the frame
     and whose cdr is the Lisp object that is the event's value.  */
  /* This field is last so that struct selection_input_event
     does not overlap with it.  */
  Lisp_Object frame_or_window;

  /* Additional event argument.  This is used for TOOL_BAR_EVENTs and
     HELP_EVENTs and avoids calling Fcons during signal handling.  */
  Lisp_Object arg;
};

#define EVENT_INIT(event) memset (&(event), 0, sizeof (struct input_event))

/* Bits in the modifiers member of the input_event structure.
   Note that reorder_modifiers assumes that the bits are in canonical
   order.

   The modifiers applied to mouse clicks are rather ornate.  The
   window-system-specific code should store mouse clicks with
   up_modifier or down_modifier set.  Having an explicit down modifier
   simplifies some of window-system-independent code; without it, the
   code would have to recognize down events by checking if the event
   is a mouse click lacking the click and drag modifiers.

   The window-system independent code turns all up_modifier events
   bits into drag_modifier, click_modifier, double_modifier, or
   triple_modifier events.  The click_modifier has no written
   representation in the names of the symbols used as event heads,
   but it does appear in the Qevent_symbol_components property of the
   event heads.  */
enum {
  up_modifier	=   1,		/* Only used on mouse buttons - always
				   turned into a click or a drag modifier
				   before lisp code sees the event.  */
  down_modifier =   2,		/* Only used on mouse buttons.  */
  drag_modifier =   4,		/* This is never used in the event
				   queue; it's only used internally by
				   the window-system-independent code.  */
  click_modifier=   8,		/* See drag_modifier.  */
  double_modifier= 16,          /* See drag_modifier.  */
  triple_modifier= 32,          /* See drag_modifier.  */

  /* The next four modifier bits are used also in keyboard events at
     the Lisp level.

     It's probably not the greatest idea to use the 2^28 bit for any
     modifier.  It may or may not be the sign bit, depending on
     FIXNUM_BITS, so using it to represent a modifier key means that
     characters thus modified have different integer equivalents
     depending on the architecture they're running on.  Oh, and
     applying XINT to a character whose 2^28 bit is set might sign-extend
     it, so you get a bunch of bits in the mask you didn't want.

     The CHAR_ macros are defined in lisp.h.  */
  alt_modifier	=  CHAR_ALT,	/* Under X, the XK_Alt_[LR] keysyms.  */
  super_modifier=  CHAR_SUPER,	/* Under X, the XK_Super_[LR] keysyms.  */
  hyper_modifier=  CHAR_HYPER,	/* Under X, the XK_Hyper_[LR] keysyms.  */
  shift_modifier=  CHAR_SHIFT,
  ctrl_modifier	=  CHAR_CTL,
  meta_modifier	=  CHAR_META	/* Under X, the XK_Meta_[LR] keysyms.  */
};

#ifdef HAVE_GPM
#include <gpm.h>
extern int handle_one_term_event (struct tty_display_info *, Gpm_Event *, struct input_event *);
#ifndef HAVE_WINDOW_SYSTEM
extern void term_mouse_moveto (int, int);
#endif

/* The device for which we have enabled gpm support.  */
extern struct tty_display_info *gpm_tty;
#endif


struct ns_display_info;
struct x_display_info;
struct w32_display_info;

/* Terminal-local parameters. */
struct terminal
{
  /* This is for Lisp; the terminal code does not refer to it.  */
  struct vectorlike_header header;

  /* Parameter alist of this terminal.  */
  Lisp_Object param_alist;

  /* List of charsets supported by the terminal.  It is set by
     Fset_terminal_coding_system_internal along with
     the member terminal_coding.  */
  Lisp_Object charset_list;

  /* This is an association list containing the X selections that
     Emacs might own on this terminal.  Each element has the form
       (SELECTION-NAME SELECTION-VALUE SELECTION-TIMESTAMP FRAME)
     SELECTION-NAME is a lisp symbol, whose name is the name of an X Atom.
     SELECTION-VALUE is the value that emacs owns for that selection.
      It may be any kind of Lisp object.
     SELECTION-TIMESTAMP is the time at which emacs began owning this
      selection, as a cons of two 16-bit numbers (making a 32 bit
      time.)
     FRAME is the frame for which we made the selection.  If there is
      an entry in this alist, then it can be assumed that Emacs owns
      that selection.
     The only (eq) parts of this list that are visible from Lisp are
    the selection-values.  */
  Lisp_Object Vselection_alist;

  /* All fields before `next_terminal' should be Lisp_Object and are traced
     by the GC.  All fields afterwards are ignored by the GC.  */

  /* Chain of all terminal devices. */
  struct terminal *next_terminal;

  /* Unique id for this terminal device. */
  int id;

  /* The number of frames that are on this terminal. */
  int reference_count;

  /* The type of the terminal device. */
  enum output_method type;

  /* The name of the terminal device.  Do not use this to uniquely
     identify a terminal; the same device may be opened multiple
     times. */
  char *name;

  /* The terminal's keyboard object. */
  struct kboard *kboard;

#ifdef HAVE_WINDOW_SYSTEM
  /* Cache of images.  */
  struct image_cache *image_cache;
#endif /* HAVE_WINDOW_SYSTEM */

  /* Device-type dependent data shared amongst all frames on this terminal. */
  union display_info
  {
    struct tty_display_info *tty;     /* termchar.h */
    struct x_display_info *x;         /* xterm.h */
    struct w32_display_info *w32;     /* w32term.h */
    struct ns_display_info *ns;       /* nsterm.h */
  } display_info;


  /* Coding-system to be used for encoding terminal output.  This
     structure contains information of a coding-system specified by
     the function `set-terminal-coding-system'.  Also see
     `safe_terminal_coding' in coding.h.  */
  struct coding_system *terminal_coding;

  /* Coding-system of what is sent from terminal keyboard.  This
     structure contains information of a coding-system specified by
     the function `set-keyboard-coding-system'.  */
  struct coding_system *keyboard_coding;

  /* Terminal characteristics. */
  /* XXX Are these really used on non-termcap displays? */

  int must_write_spaces;	/* Nonzero means spaces in the text must
				   actually be output; can't just skip over
				   some columns to leave them blank.  */
  int fast_clear_end_of_line;   /* Nonzero means terminal has a `ce' string */

  int line_ins_del_ok;          /* Terminal can insert and delete lines */
  int char_ins_del_ok;          /* Terminal can insert and delete chars */
  int scroll_region_ok;         /* Terminal supports setting the scroll
                                   window */
  int scroll_region_cost;	/* Cost of setting the scroll window,
                                   measured in characters. */
  int memory_below_frame;	/* Terminal remembers lines scrolled
                                   off bottom */

#if 0  /* These are not used anywhere. */
  /* EMACS_INT baud_rate; */	/* Output speed in baud */
  int min_padding_speed;	/* Speed below which no padding necessary. */
  int dont_calculate_costs;     /* Nonzero means don't bother computing
                                   various cost tables; we won't use them. */
#endif


  /* Window-based redisplay interface for this device (0 for tty
     devices). */
  struct redisplay_interface *rif;

  /* Frame-based redisplay interface. */

  /* Text display hooks.  */

  void (*cursor_to_hook) (struct frame *f, int vpos, int hpos);
  void (*raw_cursor_to_hook) (struct frame *, int, int);

  void (*clear_to_end_hook) (struct frame *);
  void (*clear_frame_hook) (struct frame *);
  void (*clear_end_of_line_hook) (struct frame *, int);

  void (*ins_del_lines_hook) (struct frame *f, int, int);

  void (*insert_glyphs_hook) (struct frame *f, struct glyph *s, int n);
  void (*write_glyphs_hook) (struct frame *f, struct glyph *s, int n);
  void (*delete_glyphs_hook) (struct frame *, int);

  void (*ring_bell_hook) (struct frame *f);
  void (*toggle_invisible_pointer_hook) (struct frame *f, int invisible);

  void (*reset_terminal_modes_hook) (struct terminal *);
  void (*set_terminal_modes_hook) (struct terminal *);

  void (*update_begin_hook) (struct frame *);
  void (*update_end_hook) (struct frame *);
  void (*set_terminal_window_hook) (struct frame *, int);

  /* Multi-frame and mouse support hooks.  */

  /* Return the current position of the mouse.

     Set *f to the frame the mouse is in, or zero if the mouse is in no
     Emacs frame.  If it is set to zero, all the other arguments are
     garbage.

     If the motion started in a scroll bar, set *bar_window to the
     scroll bar's window, *part to the part the mouse is currently over,
     *x to the position of the mouse along the scroll bar, and *y to the
     overall length of the scroll bar.

     Otherwise, set *bar_window to Qnil, and *x and *y to the column and
     row of the character cell the mouse is over.

     Set *time to the time the mouse was at the returned position.

     This should clear mouse_moved until the next motion
     event arrives.  */
  void (*mouse_position_hook) (struct frame **f, int,
                               Lisp_Object *bar_window,
                               enum scroll_bar_part *part,
                               Lisp_Object *x,
                               Lisp_Object *y,
                               Time *);

  /* The window system handling code should set this if the mouse has
     moved since the last call to the mouse_position_hook.  Calling that
     hook should clear this.  */
  int mouse_moved;

  /* When a frame's focus redirection is changed, this hook tells the
     window system code to re-decide where to put the highlight.  Under
     X, this means that Emacs lies about where the focus is.  */
  void (*frame_rehighlight_hook) (struct frame *);

  /* If we're displaying frames using a window system that can stack
     frames on top of each other, this hook allows you to bring a frame
     to the front, or bury it behind all the other windows.  If this
     hook is zero, that means the terminal we're displaying on doesn't
     support overlapping frames, so there's no need to raise or lower
     anything.

     If RAISE_FLAG is non-zero, F is brought to the front, before all other
     windows.  If RAISE_FLAG is zero, F is sent to the back, behind all other
     windows.  */
  void (*frame_raise_lower_hook) (struct frame *f, int raise_flag);

  /* If the value of the frame parameter changed, whis hook is called.
     For example, if going from fullscreen to not fullscreen this hook
     may do something OS dependent, like extended window manager hints on X11.  */
  void (*fullscreen_hook) (struct frame *f);

  
  /* Scroll bar hooks.  */

  /* The representation of scroll bars is determined by the code which
     implements them, except for one thing: they must be represented by
     lisp objects.  This allows us to place references to them in
     Lisp_Windows without worrying about those references becoming
     dangling references when the scroll bar is destroyed.

     The window-system-independent portion of Emacs just refers to
     scroll bars via their windows, and never looks inside the scroll bar
     representation; it always uses hook functions to do all the
     scroll bar manipulation it needs.

     The `vertical_scroll_bar' field of a Lisp_Window refers to that
     window's scroll bar, or is nil if the window doesn't have a
     scroll bar.

     The `scroll_bars' and `condemned_scroll_bars' fields of a Lisp_Frame
     are free for use by the scroll bar implementation in any way it sees
     fit.  They are marked by the garbage collector.  */


  /* Set the vertical scroll bar for WINDOW to have its upper left corner
     at (TOP, LEFT), and be LENGTH rows high.  Set its handle to
     indicate that we are displaying PORTION characters out of a total
     of WHOLE characters, starting at POSITION.  If WINDOW doesn't yet
     have a scroll bar, create one for it.  */
  void (*set_vertical_scroll_bar_hook) (struct window *window,
                                        int portion, int whole,
                                        int position);


  /* The following three hooks are used when we're doing a thorough
     redisplay of the frame.  We don't explicitly know which scroll bars
     are going to be deleted, because keeping track of when windows go
     away is a real pain - can you say set-window-configuration?
     Instead, we just assert at the beginning of redisplay that *all*
     scroll bars are to be removed, and then save scroll bars from the
     fiery pit when we actually redisplay their window.  */

  /* Arrange for all scroll bars on FRAME to be removed at the next call
     to `*judge_scroll_bars_hook'.  A scroll bar may be spared if
     `*redeem_scroll_bar_hook' is applied to its window before the judgment.

     This should be applied to each frame each time its window tree is
     redisplayed, even if it is not displaying scroll bars at the moment;
     if the HAS_SCROLL_BARS flag has just been turned off, only calling
     this and the judge_scroll_bars_hook will get rid of them.

     If non-zero, this hook should be safe to apply to any frame,
     whether or not it can support scroll bars, and whether or not it is
     currently displaying them.  */
  void (*condemn_scroll_bars_hook) (struct frame *frame);

  /* Unmark WINDOW's scroll bar for deletion in this judgment cycle.
     Note that it's okay to redeem a scroll bar that is not condemned.  */
  void (*redeem_scroll_bar_hook) (struct window *window);

  /* Remove all scroll bars on FRAME that haven't been saved since the
     last call to `*condemn_scroll_bars_hook'.

     This should be applied to each frame after each time its window
     tree is redisplayed, even if it is not displaying scroll bars at the
     moment; if the HAS_SCROLL_BARS flag has just been turned off, only
     calling this and condemn_scroll_bars_hook will get rid of them.

     If non-zero, this hook should be safe to apply to any frame,
     whether or not it can support scroll bars, and whether or not it is
     currently displaying them.  */
  void (*judge_scroll_bars_hook) (struct frame *FRAME);


  /* Called to read input events.

     TERMINAL indicates which terminal device to read from.  Input
     events should be read into BUF, the size of which is given in
     SIZE.  EXPECTED is non-zero if the caller suspects that new input
     is available.

     A positive return value indicates that that many input events
     where read into BUF.
     Zero means no events were immediately available.
     A value of -1 means a transient read error, while -2 indicates
     that the device was closed (hangup), and it should be deleted.

     XXX Please note that a non-zero value of EXPECTED only means that
     there is available input on at least one of the currently opened
     terminal devices -- but not necessarily on this device.
     Therefore, in most cases EXPECTED should be simply ignored.

     XXX This documentation needs to be updated.  */
  int (*read_socket_hook) (struct terminal *terminal,
                           int expected,
                           struct input_event *hold_quit);

  /* Called when a frame's display becomes entirely up to date.  */
  void (*frame_up_to_date_hook) (struct frame *);


  /* Called to delete the device-specific portions of a frame that is
     on this terminal device. */
  void (*delete_frame_hook) (struct frame *);

  /* Called after the last frame on this terminal is deleted, or when
     the display device was closed (hangup).

     If this is NULL, then the generic delete_terminal is called
     instead.  Otherwise the hook must call delete_terminal itself.

     The hook must check for and close any live frames that are still
     on the terminal.  delete_frame ensures that there are no live
     frames on the terminal when it calls this hook, so infinite
     recursion is prevented.  */
  void (*delete_terminal_hook) (struct terminal *);
};


/* Chain of all terminal devices currently in use. */
extern struct terminal *terminal_list;

#define FRAME_MUST_WRITE_SPACES(f) ((f)->terminal->must_write_spaces)
#define FRAME_FAST_CLEAR_END_OF_LINE(f) ((f)->terminal->fast_clear_end_of_line)
#define FRAME_LINE_INS_DEL_OK(f) ((f)->terminal->line_ins_del_ok)
#define FRAME_CHAR_INS_DEL_OK(f) ((f)->terminal->char_ins_del_ok)
#define FRAME_SCROLL_REGION_OK(f) ((f)->terminal->scroll_region_ok)
#define FRAME_SCROLL_REGION_COST(f) ((f)->terminal->scroll_region_cost)
#define FRAME_MEMORY_BELOW_FRAME(f) ((f)->terminal->memory_below_frame)

#define FRAME_TERMINAL_CODING(f) ((f)->terminal->terminal_coding)
#define FRAME_KEYBOARD_CODING(f) ((f)->terminal->keyboard_coding)

#define TERMINAL_TERMINAL_CODING(d) ((d)->terminal_coding)
#define TERMINAL_KEYBOARD_CODING(d) ((d)->keyboard_coding)

#define FRAME_RIF(f) ((f)->terminal->rif)

#define FRAME_TERMINAL(f) ((f)->terminal)

/* Return true if the terminal device is not suspended. */
#define TERMINAL_ACTIVE_P(d) (((d)->type != output_termcap && (d)->type !=output_msdos_raw) || (d)->display_info.tty->input)

extern struct terminal *get_terminal (Lisp_Object terminal, int);
extern struct terminal *create_terminal (void);
extern void delete_terminal (struct terminal *);

/* The initial terminal device, created by initial_term_init. */
extern struct terminal *initial_terminal;

extern unsigned char *encode_terminal_code (struct glyph *, int,
					    struct coding_system *);

#ifdef HAVE_GPM
extern void close_gpm (int gpm_fd);
#endif
