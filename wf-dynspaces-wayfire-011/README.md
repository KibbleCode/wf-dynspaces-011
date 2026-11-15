# Experimental wf-dynspaces adaptation for Wayfire 0.11

**STATUS: UNCOMPILED/UNVERIFIED. DO NOT INSTALL OR ENABLE YET.**

The uploaded `wf-dynspaces` is an older plugin which references removed APIs such as `wayfire/workspace-manager.hpp`, `workspace_implementation_t`, the old layer functions, and legacy signal callback registration. Wayfire 0.11 replaces these interfaces with `workspace_set_t`, `per_output_plugin_t` and typed signal connections.

This is an **isolated proof-of-concept rewrite**, not a complete port of the original. In particular:

- Watches *maximize* (`view_tiled_signal`) instead of true fullscreen, preserving decorations.
- Allocates one additional grid column per maximized app.
- Moves the app to the newly added workspace and follows it.
- On restore or close, frees the slot and returns to `(0,0)`.
- Does **not** capture direct drag gestures, or remove desktop decorations.
- Records the existing workspace grid width and height instead of resetting them on startup.
- Does **not** preserve original non-main workspace positions if windows were already on other workspaces.
- May conflict with other plugins managing tiled or fullscreen state; runtime compatibility untested.

## Compile test only

Do NOT run `wayfire-plugin install`, do NOT change `~/.config/wayfire.ini`.

```sh
cd ~/Downloads/wf-dynspaces-wayfire-011
meson setup build
meson compile -C build
```

If compile fails, send the **first compiler error and about 25 lines below it**. If it compiles, it has **not** been runtime-verified; first test would require more scrutiny of cleanup and interactions before loading it into your live compositor.

Original source: https://codeberg.org/valpackett/wf-dynspaces (Unlicense).
