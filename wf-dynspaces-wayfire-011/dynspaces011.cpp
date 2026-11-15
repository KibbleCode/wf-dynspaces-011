// Experimental Wayfire 0.11 compatibility rewrite of wf-dynspaces.
// Source inspiration: valpackett/wf-dynspaces (Unlicense).
// This version intentionally manages MAXIMIZED windows, rather than true fullscreen.
// It does not modify pixdecor or request fullscreen.

#include <algorithm>
#include <vector>

#include <wayfire/output.hpp>
#include <wayfire/per-output-plugin.hpp>
#include <wayfire/plugin.hpp>
#include <wayfire/signal-definitions.hpp>
#include <wayfire/toplevel-view.hpp>
#include <wayfire/workspace-set.hpp>

namespace {
constexpr uint32_t ALL_EDGES = 15;

class dynspaces_output_t : public wf::per_output_plugin_instance_t
{
    std::vector<wayfire_toplevel_view> managed;
    int initial_width = 1;
    int initial_height = 1;
    bool in_update = false;

    wf::signal::connection_t<wf::view_tiled_signal> on_tiled =
        [this] (wf::view_tiled_signal *event)
    {
        if (in_update || !event || !event->view)
        {
            return;
        }

        const auto edges = event->new_edges;
        const auto old_edges = event->old_edges;
        if ((edges == ALL_EDGES) && (old_edges != ALL_EDGES))
        {
            add_maximized(event->view);
        } else if ((old_edges == ALL_EDGES) && (edges != ALL_EDGES))
        {
            remove_managed(event->view, true);
        }
    };

    wf::signal::connection_t<wf::view_unmapped_signal> on_unmapped =
        [this] (wf::view_unmapped_signal *event)
    {
        if (in_update || !event || !event->view)
        {
            return;
        }

        for (auto view : managed)
        {
            if (view.get() == event->view.get())
            {
                remove_managed(view, false);
                break;
            }
        }
    };

    void add_maximized(wayfire_toplevel_view view)
    {
        if (!view || (view->get_output() != output))
        {
            return;
        }
        if (std::find(managed.begin(), managed.end(), view) != managed.end())
        {
            return;
        }
        auto ws = output->wset();
        if (!ws)
        {
            return;
        }

        in_update = true;
        managed.push_back(view);
        const int new_index = initial_width + (int)managed.size() - 1;
        ws->set_workspace_grid_size({initial_width + (int)managed.size(), initial_height});
        ws->move_to_workspace(view, {new_index, 0});
        ws->request_workspace({new_index, 0});
        in_update = false;
    }

    void remove_managed(wayfire_toplevel_view view, bool window_survives)
    {
        auto it = std::find(managed.begin(), managed.end(), view);
        if (it == managed.end())
        {
            return;
        }

        auto ws = output->wset();
        if (!ws)
        {
            return;
        }
        in_update = true;
        const size_t position = std::distance(managed.begin(), it);
        if (window_survives)
        {
            ws->move_to_workspace(view, {0, 0});
        }
        // Move only windows tracked by this plugin, leave other apps alone.
        for (size_t i = position + 1; i < managed.size(); ++i)
        {
            auto other = managed[i];
            if (other)
            {
                const int new_index = initial_width + (int)i - 1;
                ws->move_to_workspace(other, {new_index, 0});
            }
        }
        managed.erase(it);
        ws->set_workspace_grid_size({initial_width + (int)managed.size(), initial_height});
        ws->request_workspace({0, 0});
        in_update = false;
    }

  public:
    void init() override
    {
        if (!output->wset())
        {
            return;
        }
        const auto grid = output->wset()->get_workspace_grid_size();
        initial_width = std::max(1, grid.width);
        initial_height = std::max(1, grid.height);
        output->connect(&on_tiled);
        output->connect(&on_unmapped);
    }

    void fini() override
    {
        on_tiled.disconnect();
        on_unmapped.disconnect();
        auto ws = output->wset();
        if (!ws)
        {
            return;
        }
        for (auto view : managed)
        {
            if (view)
            {
                ws->move_to_workspace(view, {0, 0});
            }
        }
        managed.clear();
        ws->set_workspace_grid_size({initial_width, initial_height});
        ws->request_workspace({0, 0});
    }
};

using dynspaces_plugin_t = wf::per_output_plugin_t<dynspaces_output_t>;
}

DECLARE_WAYFIRE_PLUGIN(dynspaces_plugin_t);
