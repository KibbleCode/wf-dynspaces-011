# Source compatibility audit

Source: user-provided `wf-dynspaces-source.tar.gz`, containing `dynspaces.cpp` and `meson.build` (278 and 13 lines).

- Original used `#include <wayfire/workspace-manager.hpp>`; Wayfire 0.11 exports `wayfire/workspace-set.hpp`.
- Original used `output->workspace`; current per-output API uses `output->wset()`.
- Original used `wf::workspace_implementation_t` and `set_workspace_implementation`; no direct equivalent in current workspace-set API.
- Original used legacy `wf::signal_connection_t`, `wf::signal_data_t`, and signal strings; Wayfire 0.11 uses typed `wf::signal::connection_t<...>` and `output->connect(&connection)`.
- Original used `get_views_in_layer()` and `get_views_on_workspace()`; 0.11 workspace-set API provides `get_views(flags, optional_workspace)`.
- Original used `wf::grid::grid_animation_t`, a plugin-specific animation helper; not carried over in this candidate.
- Original relies on integer geometry and `get_screen_size()`; 0.11 uses fractional geometry.
- Original handles true fullscreen requests rather than clicking maximize. That differs from the user's desired behavior.
- Original source zip lacks `dynspaces.xml`; the build file references that metadata, so the original archive alone is incomplete for a full installation.
- Dependency on unversioned `wlroots` was fixed to `wlroots-0.20`, but compilation then failed at the removed workspace manager header. This proof of concept relies only on the Wayfire dependency.
