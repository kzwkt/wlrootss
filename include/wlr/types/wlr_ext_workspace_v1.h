/*
 * This an unstable interface of wlroots. No guarantees are made regarding the
 * future consistency of this API.
 */
#ifndef WLR_USE_UNSTABLE
#error "Add -DWLR_USE_UNSTABLE to enable unstable wlroots features"
#endif

#ifndef WLR_TYPES_WLR_WORKSPACE_V1_H
#define WLR_TYPES_WLR_WORKSPACE_V1_H

#include <wayland-server-core.h>
#include <wlr/types/wlr_output.h>

struct wlr_workspace_manager_v1 {
	struct wl_event_loop *event_loop;
	struct wl_event_source *idle_source;

	struct wl_global *global;
	struct wl_list resources; // wl_resource_get_link
	struct wl_list groups; // wlr_workspace_group_handle_v1::link

	struct wl_listener display_destroy;

	struct {
		struct wl_signal commit; // wlr_workspace_manager_v1
		struct wl_signal destroy;
	} events;

	void *data;
};

struct wlr_workspace_group_handle_v1 {
	struct wl_list link; // wlr_workspace_manager_v1::groups
	struct wl_list resources; // wl_resource_get_link

	struct wl_list workspaces; // wlr_workspace_handle_v1::link
	struct wl_list outputs; // wlr_workspace_group_handle_v1_output::link

	struct wlr_workspace_manager_v1 *manager;

	struct {
		struct wl_signal destroy;
	} events;

	void *data;
};

struct wlr_workspace_group_handle_v1_output {
	struct wl_list link; // wlr_workspace_group_handle_v1::outputs
	struct wl_listener output_destroy;
	struct wlr_output *output;

	struct wlr_workspace_group_handle_v1 *group_handle;
};

enum wlr_workspace_handle_v1_state
{
	WLR_WORKSPACE_HANDLE_V1_STATE_ACTIVE = 1 << 0,
};

struct wlr_workspace_handle_v1 {
	struct wl_list link; // wlr_workspace_group_handle_v1::workspaces
	struct wl_list resources;

	struct wlr_workspace_group_handle_v1 *group;

	// request from the client
	uint32_t pending, current;

	// set by the compositor
	uint32_t server_state;

	char *name;
	struct wl_array coordinates;

	struct {
		struct wl_signal destroy;
	} events;

	void *data;
};

struct wlr_workspace_manager_v1 *wlr_workspace_manager_v1_create(
		struct wl_display *display);

struct wlr_workspace_group_handle_v1 *wlr_workspace_group_handle_v1_create(
		struct wlr_workspace_manager_v1 *manager);

/**
 * Destroy the workspace group and all workspaces inside it.
 */
void wlr_workspace_group_handle_v1_destroy(
	struct wlr_workspace_group_handle_v1 *group);

/**
 * Create a new workspace in the workspace group.
 * Note that the compositor must set the workspace name immediately after
 * creating it.
 */
struct wlr_workspace_handle_v1 *wlr_workspace_handle_v1_create(
		struct wlr_workspace_group_handle_v1 *group);

void wlr_workspace_handle_v1_destroy(
	struct wlr_workspace_handle_v1 *workspace);

void wlr_workspace_group_handle_v1_output_enter(
		struct wlr_workspace_group_handle_v1 *group, struct wlr_output *output);

void wlr_workspace_group_handle_v1_output_leave(
		struct wlr_workspace_group_handle_v1 *group, struct wlr_output *output);

void wlr_workspace_handle_v1_set_name(struct wlr_workspace_handle_v1 *workspace,
		const char* name);

void wlr_workspace_handle_v1_set_coordinates(
		struct wlr_workspace_handle_v1 *workspace, struct wl_array *coordinates);

void wlr_workspace_handle_v1_set_active(
		struct wlr_workspace_handle_v1 *workspace, bool active);

#endif