#include "nuklear.h"
#include "nuklear_internal.h"

/* ===============================================================
 *
 *                             DOCKING
 *
 * ===============================================================*/

NK_API void
nk_dockspace_init(struct nk_context* ctx, int* width, int* height)
{
    int w = 0, h = 0;

    if (w == 0) w = *width;
    else *width = w;
    if (h == 0) h = *height;
    else *height = h;

    ctx->display_size.x = (float)w;
    ctx->display_size.y = (float)h;
    ctx->dock_restored = nk_false;

    ctx->dockable_space.x = 0;
    ctx->dockable_space.y = 0;
    ctx->dockable_space.w = (float)w;
    ctx->dockable_space.h = (float)h;

    ctx->dck.windows = 0;
    ctx->dck.max_windows = 5;
    ctx->dck.num_windows = 0;
    ctx->dck.count = 0;
    ctx->dck.flags = 0;
    ctx->dck.next = 0;
}

NK_API void
nk_dockspace_add_window(struct nk_context* ctx, struct nk_window* win)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);

    const struct nk_vec2 mouse_pos = ctx->input.mouse.pos;
    struct nk_rect top_bound = nk_dockspace_dock_highlight(ctx, NK_DOCK_TOP);
    struct nk_rect left_bound = nk_dockspace_dock_highlight(ctx, NK_DOCK_LEFT);
    struct nk_rect right_bound = nk_dockspace_dock_highlight(ctx, NK_DOCK_RIGHT);
    struct nk_rect bottom_bound = nk_dockspace_dock_highlight(ctx, NK_DOCK_BOTTOM);
    struct nk_dockspace* dck = &ctx->dck;

    if ((mouse_pos.x > top_bound.x) && (mouse_pos.x < top_bound.x + top_bound.w) &&
        (mouse_pos.y > top_bound.y) && (mouse_pos.y < top_bound.y + top_bound.h)) {
        nk_dockspace_set(ctx, win, NK_DOCK_TOP);
    }
    else if ((mouse_pos.x > left_bound.x) && (mouse_pos.x < left_bound.x + left_bound.w) &&
        (mouse_pos.y > left_bound.y) && (mouse_pos.y < left_bound.y + left_bound.h)) {
        nk_dockspace_set(ctx, win, NK_DOCK_LEFT);
    }
    else if ((mouse_pos.x > right_bound.x) && (mouse_pos.x < right_bound.x + right_bound.w) &&
        (mouse_pos.y > right_bound.y) && (mouse_pos.y < right_bound.y + right_bound.h)) {
        nk_dockspace_set(ctx, win, NK_DOCK_RIGHT);
    }
    else if ((mouse_pos.x > bottom_bound.x) && (mouse_pos.x < bottom_bound.x + bottom_bound.w) &&
        (mouse_pos.y > bottom_bound.y) && (mouse_pos.y < bottom_bound.y + bottom_bound.h)) {
        nk_dockspace_set(ctx, win, NK_DOCK_BOTTOM);
    }
    else {
        struct nk_dockspace* parent = nk_dockspace_find_window(ctx, win);
        struct nk_rect inner_bound = nk_dockspace_dock_inner_highlight(ctx);
        if (inner_bound.w != 0 && inner_bound.h != 0) {
            while (inner_bound.x < dck->bounds.x || inner_bound.x >= dck->bounds.x + dck->bounds.w
                || inner_bound.y < dck->bounds.y || inner_bound.y >= dck->bounds.y + dck->bounds.h) {
                dck = dck->next;
                if (dck == 0)
                    break;
            }
            if (dck == parent || dck == 0) {
                nk_dockspace_assign_window(ctx, parent, win, inner_bound);
                return;
            }
            nk_dockspace_append_window(ctx, dck, win, inner_bound);
            int i = 0;
            struct nk_dockspace* temp = &ctx->dck;
            while (temp != 0) {
                i++;
                temp = temp->next;
            }
            nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
        }
        else if (parent) {
            nk_dockspace_assign_window(ctx, parent, win, inner_bound);
        }
        return;
    }
}

NK_API void
nk_dockspace_remove_window(struct nk_context* ctx, struct nk_window* win)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);

    struct nk_dockspace* dock_parent = &ctx->dck;
    if (dock_parent->windows == 0)
        return;
    struct nk_dockspace* dock_child = dock_parent;

    while (dock_child != 0) {
        if (dock_child->windows == 0)
            return;
        if (dock_child->num_windows == 1) {
            if (dock_child->windows->win == win) {
                win->bounds.w = dock_child->windows->originalWidth;
                win->bounds.h = dock_child->windows->originalHeight;
                if (dock_parent == dock_child) {
                    struct nk_dockspace* temp = ctx->dck.next;
                    ctx->dck = *(ctx->dck.next);
                    dock_child = 0;
                    dock_parent = 0;
                    free(temp);
                }
                else {
                    dock_parent->next = dock_child->next;
                    free(dock_child->windows);
                    free(dock_child);
                }
                return;
            }
        }
        else {
            for (unsigned int i = 0; i < dock_child->num_windows; i++) {
                if (dock_child->windows[i].win == win) {
                    win->bounds.w = dock_child->windows[i].originalWidth;
                    win->bounds.h = dock_child->windows[i].originalHeight;
                    for (unsigned int j = i; j < dock_child->num_windows; j++) {
                        dock_child->windows[j] = dock_child->windows[j + 1];
                    }
                    dock_child->num_windows--;
                }
            }
        }
        if (dock_parent == dock_child) {
            dock_child = dock_parent->next;
        }
        else {
            dock_parent = dock_child;
            dock_child = dock_parent->next;
        }
    }
}

NK_API void
nk_dockspace_relocate(struct nk_context* ctx, struct nk_dockspace* dck, nk_flags flag)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    if (ctx->current->flags & NK_WINDOW_DOCK_SUPER) {
        struct nk_dockspace* dock_parent = &ctx->dck;
        struct nk_dockspace* dock_child = dock_parent;

        while (dock_child != dck) {
            dock_parent = dock_child;
            dock_child = dock_parent->next;
        }
        if (dock_child == 0) return;
        struct nk_dockspace* final_dock = dock_child;
        struct nk_dockspace* final_next = final_dock;
        while (final_next->num_windows != 0) {
            final_dock = final_next;
            final_next = final_next->next;
        }

        struct nk_dockspace* setting_dock;
        if (dock_parent == dock_child) {
            if (dock_child->next->num_windows == 0) {
                setting_dock = &ctx->dck;
            }
            else if (dock_child->next == final_dock) {
                struct nk_dockspace temp = *dock_child;
                ctx->dck = *(ctx->dck.next);
                free(final_dock);

                ctx->dck.next->bounds = temp.bounds;
                ctx->dck.next->flags = temp.flags;
                ctx->dck.next->max_windows = temp.max_windows;
                ctx->dck.next->next = temp.next;
                ctx->dck.next->num_windows = temp.num_windows;
                ctx->dck.next->windows = temp.windows;

                setting_dock = ctx->dck.next;
                setting_dock->next = (struct nk_dockspace*)malloc(sizeof(struct nk_dockspace));
                setting_dock->next->windows = 0;
                setting_dock->next->num_windows = 0;
                setting_dock->next->flags = 0;
                setting_dock->next->next = 0;
            }
            else {
                struct nk_dockspace temp = *dock_child;
                struct nk_dockspace* to_remove = dock_child->next;
                ctx->dck = *(ctx->dck.next);
                free(to_remove);

                final_next->bounds = temp.bounds;
                final_next->flags = temp.flags;
                final_next->max_windows = temp.max_windows;
                final_next->next = temp.next;
                final_next->num_windows = temp.num_windows;
                final_next->windows = temp.windows;

                setting_dock = final_next;
                setting_dock->next = (struct nk_dockspace*)malloc(sizeof(struct nk_dockspace));
                setting_dock->next->windows = 0;
                setting_dock->next->num_windows = 0;
                setting_dock->next->flags = 0;
                setting_dock->next->next = 0;
            }
        }
        else {
            if (dock_child == final_dock) {
                setting_dock = dock_child;
            }
            else {
                dock_parent->next = dock_child->next;
                final_dock->next = dock_child;
                dock_child->next = final_next;

                setting_dock = dock_child;
            }
        }

        if (flag & NK_DOCK_TOP) {
            setting_dock->bounds.h = setting_dock->windows[0].originalHeight;
            setting_dock->flags &= ~(NK_DOCK_LEFT | NK_DOCK_RIGHT | NK_DOCK_BOTTOM);
            setting_dock->flags |= NK_DOCK_TOP;
        }
        else if (flag & NK_DOCK_LEFT) {
            setting_dock->bounds.w = setting_dock->windows[0].originalWidth;
            setting_dock->flags &= ~(NK_DOCK_TOP | NK_DOCK_RIGHT | NK_DOCK_BOTTOM);
            setting_dock->flags |= NK_DOCK_LEFT;
        }
        else if (flag & NK_DOCK_RIGHT) {
            setting_dock->bounds.w = setting_dock->windows[0].originalWidth;
            setting_dock->flags &= ~(NK_DOCK_TOP | NK_DOCK_LEFT | NK_DOCK_BOTTOM);
            setting_dock->flags |= NK_DOCK_RIGHT;
        }
        else if (flag & NK_DOCK_BOTTOM) {
            setting_dock->bounds.h = setting_dock->windows[0].originalHeight;
            setting_dock->flags &= ~(NK_DOCK_TOP | NK_DOCK_LEFT | NK_DOCK_RIGHT);
            setting_dock->flags |= NK_DOCK_BOTTOM;
        }
        nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
    }
}

NK_API void
nk_dockspace_adjust(struct nk_context* ctx, float width, float height)
{
    NK_ASSERT(ctx);

    ctx->display_size.x = width;
    ctx->display_size.y = height;

    struct nk_rect dockable_space = { 0.0f, 0.0f, width, height };
    struct nk_vec2 min_size = ctx->style.window.min_size;

    struct nk_dockspace* cur = &ctx->dck;
    while (cur && cur->num_windows != 0) {
        nk_dockspace_set_count(ctx, cur);
        cur = cur->next;
    }

    struct nk_dockspace* dck = &ctx->dck;
    while (dck->num_windows != 0) {
        int win_count = 0;
        float w = 0.0f;
        float h = 0.0f;

        for (unsigned int i = 0; i < dck->num_windows; i++) {
            const struct nk_rect dockable = dockable_space;
            if (dck->flags & NK_DOCK_TOP) {
                if (win_count == 0) {
                    dck->bounds.x = dockable.x;
                    dck->bounds.y = dockable.y;
                    dck->bounds.w = dockable.w;
                    dck->bounds.h = dck->windows[0].win->bounds.h;
                    dck->bounds.h = dck->bounds.y + dck->bounds.h >
                        ctx->dockable_space.y + ctx->dockable_space.h - dck->count * min_size.y ?
                        ctx->dockable_space.y + ctx->dockable_space.h - dck->bounds.y - dck->count * min_size.y :
                        dck->bounds.h;
                    dck->bounds.h = dck->bounds.h < min_size.y ? min_size.y : dck->bounds.h;

                    dockable_space.y += dck->bounds.h;
                    dockable_space.h -= dck->bounds.h;
                }
                win_count++;
                dck->windows[i].win->bounds.x = dck->bounds.x + w;
                dck->windows[i].win->bounds.y = dck->bounds.y;
                dck->windows[i].win->bounds.h = dck->bounds.h;
                if (i == dck->num_windows - 1) {
                    dck->windows[i].win->bounds.w = dck->bounds.w - w;
                    break;
                }
                else {
                    dck->windows[i].win->bounds.w = dck->windows[i].win->bounds.x + dck->windows[i].win->bounds.w >
                        dck->bounds.x + dck->bounds.w - min_size.x * (dck->num_windows - win_count) ?
                        dck->bounds.x + dck->bounds.w - min_size.x * (dck->num_windows - win_count) - dck->windows[i].win->bounds.x :
                        dck->windows[i].win->bounds.w;
                    dck->windows[i].win->bounds.w = dck->windows[i].win->bounds.w <= min_size.x ?
                        min_size.x : dck->windows[i].win->bounds.w;
                }
                w += dck->windows[i].win->bounds.w;
            }
            else if (dck->flags & NK_DOCK_LEFT) {
                if (win_count == 0) {
                    dck->bounds.x = dockable.x;
                    dck->bounds.y = dockable.y;
                    dck->bounds.h = dockable.h;
                    dck->bounds.w = dck->windows[0].win->bounds.w;
                    dck->bounds.w = dck->bounds.x + dck->bounds.w >
                        ctx->dockable_space.x + ctx->dockable_space.w - dck->count * min_size.x ?
                        ctx->dockable_space.x + ctx->dockable_space.w - dck->bounds.x - dck->count * min_size.x :
                        dck->bounds.w;
                    dck->bounds.w = dck->bounds.w < min_size.x ? min_size.x : dck->bounds.w;

                    dockable_space.x += dck->bounds.w;
                    dockable_space.w -= dck->bounds.w;
                }
                win_count++;
                dck->windows[i].win->bounds.x = dck->bounds.x;
                dck->windows[i].win->bounds.y = dck->bounds.y + h;
                dck->windows[i].win->bounds.w = dck->bounds.w;
                if (i == dck->num_windows - 1) {
                    dck->windows[i].win->bounds.h = dck->bounds.h - h;
                    break;
                }
                else {
                    dck->windows[i].win->bounds.h = dck->windows[i].win->bounds.h <= min_size.y ?
                        min_size.y : dck->windows[i].win->bounds.h;
                    dck->windows[i].win->bounds.h = dck->windows[i].win->bounds.y + dck->windows[i].win->bounds.h >
                        dck->bounds.y + dck->bounds.h - min_size.y * (dck->num_windows - win_count) ?
                        dck->bounds.y + dck->bounds.h - min_size.y * (dck->num_windows - win_count) - dck->windows[i].win->bounds.y :
                        dck->windows[i].win->bounds.h;
                }
                if ((dck->windows[i].win->layout->flags & NK_WINDOW_MINIMIZED &&
                    !(dck->windows[i].win->flags & NK_WINDOW_MINIMIZED)) ||
                    (dck->windows[i].win->flags & NK_WINDOW_MINIMIZED &&
                        !(dck->windows[i].win->layout->flags & NK_WINDOW_MINIMIZED))) {
                    //Minimized window
                    float header = ctx->style.font->height + 2.0f * ctx->style.window.header.padding.y +
                        (2.0f * ctx->style.window.header.label_padding.y);
                    h += header;
                }
                else
                    h += dck->windows[i].win->bounds.h;
            }
            else if (dck->flags & NK_DOCK_RIGHT) {
                if (win_count == 0) {
                    dck->bounds.w = dck->windows[0].win->bounds.w;
                    dck->bounds.x = dockable.x + dockable.w - dck->bounds.w;
                    dck->bounds.y = dockable.y;
                    dck->bounds.h = dockable.h;
                    dck->bounds.x = dck->bounds.x < dockable_space.x + dck->count * min_size.x ?
                        dockable_space.x + dck->count * min_size.x : dck->bounds.x;
                    dck->bounds.w = dockable.x + dockable.w - dck->bounds.x;

                    dockable_space.w -= dck->bounds.w;
                }
                win_count++;
                dck->windows[i].win->bounds.x = dck->bounds.x;
                dck->windows[i].win->bounds.y = dck->bounds.y + h;
                dck->windows[i].win->bounds.w = dck->bounds.w;
                if (i == dck->num_windows - 1) {
                    dck->windows[i].win->bounds.h = dck->bounds.h - h;
                    break;
                }
                else {
                    dck->windows[i].win->bounds.h = dck->windows[i].win->bounds.h <= min_size.y ?
                        min_size.y : dck->windows[i].win->bounds.h;
                    dck->windows[i].win->bounds.h = dck->windows[i].win->bounds.y + dck->windows[i].win->bounds.h >
                        dck->bounds.y + dck->bounds.h - min_size.y * (dck->num_windows - win_count) ?
                        dck->bounds.y + dck->bounds.h - min_size.y * (dck->num_windows - win_count) - dck->windows[i].win->bounds.y :
                        dck->windows[i].win->bounds.h;
                }
                if ((dck->windows[i].win->layout->flags & NK_WINDOW_MINIMIZED &&
                    !(dck->windows[i].win->flags & NK_WINDOW_MINIMIZED)) ||
                    (dck->windows[i].win->flags & NK_WINDOW_MINIMIZED &&
                        !(dck->windows[i].win->layout->flags & NK_WINDOW_MINIMIZED))) {
                    //Minimized window
                    float header = ctx->style.font->height + 2.0f * ctx->style.window.header.padding.y +
                        (2.0f * ctx->style.window.header.label_padding.y);
                    h += header;
                }
                else
                    h += dck->windows[i].win->bounds.h;
            }
            else if (dck->flags & NK_DOCK_BOTTOM) {
                if (win_count == 0) {
                    dck->bounds.h = dck->windows[0].win->bounds.h;
                    dck->bounds.x = dockable.x;
                    dck->bounds.y = dockable.y + dockable.h - dck->bounds.h;
                    dck->bounds.w = dockable.w;
                    dck->bounds.y = dck->bounds.y < ctx->dockable_space.y + dck->count * min_size.y ?
                        ctx->dockable_space.y + +dck->count * min_size.y : dck->bounds.y;
                    dck->bounds.h = dockable.y + dockable.h - dck->bounds.y;

                    dockable_space.h -= dck->bounds.h;
                }
                win_count++;
                dck->windows[i].win->bounds.x = dck->bounds.x + w;
                dck->windows[i].win->bounds.y = dck->bounds.y;
                dck->windows[i].win->bounds.h = dck->bounds.h;
                if (i == dck->num_windows - 1) {
                    dck->windows[i].win->bounds.w = dck->bounds.w - w;
                    break;
                }
                else {
                    dck->windows[i].win->bounds.w = dck->windows[i].win->bounds.x + dck->windows[i].win->bounds.w >
                        dck->bounds.x + dck->bounds.w - min_size.x * (dck->num_windows - win_count) ?
                        dck->bounds.x + dck->bounds.w - min_size.x * (dck->num_windows - win_count) - dck->windows[i].win->bounds.x :
                        dck->windows[i].win->bounds.w;
                    dck->windows[i].win->bounds.w = dck->windows[i].win->bounds.w <= min_size.x ?
                        min_size.x : dck->windows[i].win->bounds.w;
                }
                w += dck->windows[i].win->bounds.w;
            }
        }
        dck = dck->next;
        if (dck == 0)
            return;
    }
    ctx->dockable_space = dockable_space;
}

NK_API struct nk_rect
nk_dockspace_set_highlight(struct nk_context* ctx, struct nk_rect* left, struct nk_rect* right,
    struct nk_rect* top, struct nk_rect* bottom, struct nk_rect* inner)
{
    NK_ASSERT(ctx);

    *left = nk_dockspace_dock_highlight(ctx, NK_DOCK_LEFT);
    *right = nk_dockspace_dock_highlight(ctx, NK_DOCK_RIGHT);
    *top = nk_dockspace_dock_highlight(ctx, NK_DOCK_TOP);
    *bottom = nk_dockspace_dock_highlight(ctx, NK_DOCK_BOTTOM);
    *inner = nk_dockspace_dock_inner_highlight(ctx);
    struct nk_rect selection = { 0, 0, 0, 0 };

    const struct nk_rect dockable = ctx->dockable_space;
    const struct nk_vec2 mouse = ctx->input.mouse.pos;

    if (mouse.x > left->x && mouse.x < left->x + left->w &&
        mouse.y > left->y && mouse.y < left->y + left->h) {
        selection.x = dockable.x;
        selection.w = left->w;
        selection.y = dockable.y;
        selection.h = dockable.h;
    }
    else if (mouse.x > right->x && mouse.x < right->x + right->w &&
        mouse.y > right->y && mouse.y < right->y + right->h) {
        selection.x = right->x;
        selection.w = right->w;
        selection.y = dockable.y;
        selection.h = dockable.h;
    }
    else if (mouse.x > top->x && mouse.x < top->x + top->w &&
        mouse.y > top->y && mouse.y < top->y + top->h) {
        selection.x = dockable.x;
        selection.w = dockable.w;
        selection.y = dockable.y;
        selection.h = top->h;
    }
    else if (mouse.x > bottom->x && mouse.x < bottom->x + bottom->w &&
        mouse.y > bottom->y && mouse.y < bottom->y + bottom->h) {
        selection.x = dockable.x;
        selection.w = dockable.w;
        selection.y = bottom->y;
        selection.h = bottom->h;
    }
    return selection;
}

NK_API struct nk_rect
nk_dockspace_dock_inner_highlight(struct nk_context* ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    const struct nk_vec2 mouse_pos = ctx->input.mouse.pos;
    struct nk_dockspace* dck = &ctx->dck;
    struct nk_dockspace* curr_dock = nk_dockspace_find_window(ctx, ctx->current);
    struct nk_rect inner_bound = { 0, 0, 0, 0 };

    while (dck != 0) {
        if ((curr_dock && curr_dock->flags & NK_DOCK_LOCK && dck != curr_dock) ||
            (dck->flags & NK_DOCK_LOCK && dck != curr_dock)) {
            dck = dck->next;
            continue;
        }
        for (unsigned int i = 0; i < dck->num_windows; i++) {
            if (dck->windows[i].win == ctx->current) {
                continue;
            }

            struct nk_rect bounds = dck->windows[i].win->bounds;
            if (mouse_pos.x > bounds.x && mouse_pos.x < bounds.x + bounds.w
                && mouse_pos.y > bounds.y && mouse_pos.y < bounds.y + bounds.h) {
                if (ctx->current->flags & NK_WINDOW_DOCK_SUPER) {
                    if (dck->flags & NK_DOCK_HAS_SUPER) {
                        return inner_bound;
                    }
                    else if (nk_dockspace_find_window(ctx, ctx->current)) {
                        return inner_bound;
                    }
                    else {
                        if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                            inner_bound.x = dck->windows[0].win->bounds.x;
                            inner_bound.y = dck->windows[0].win->bounds.y;
                            inner_bound.w = ctx->current->bounds.w;
                            inner_bound.h = dck->windows[0].win->bounds.h;
                        }
                        else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                            inner_bound.x = dck->windows[0].win->bounds.x;
                            inner_bound.y = dck->windows[0].win->bounds.y;
                            inner_bound.w = dck->windows[0].win->bounds.w;
                            inner_bound.h = ctx->current->bounds.h;
                        }
                    }
                }
                else if (dck->windows[i].win->flags & NK_WINDOW_DOCK_SUPER) {
                    if (dck->num_windows == 1) {
                        if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                            inner_bound.x = bounds.x + dck->windows[i].originalWidth;
                            inner_bound.y = bounds.y;
                            inner_bound.w = bounds.w - dck->windows[i].originalWidth;
                            inner_bound.h = bounds.h;
                        }
                        else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                            inner_bound.x = bounds.x;
                            inner_bound.y = bounds.y + dck->windows[i].originalHeight;
                            inner_bound.w = bounds.w;
                            inner_bound.h = bounds.h - dck->windows[i].originalHeight;
                        }
                    }
                    continue;
                }
                else if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                    if (mouse_pos.x >= bounds.x && mouse_pos.x < bounds.x + bounds.w / 2) {
                        inner_bound.x = bounds.x;
                        inner_bound.y = bounds.y;
                        inner_bound.w = bounds.w / 2;
                        inner_bound.h = bounds.h;
                    }
                    else if (mouse_pos.x >= bounds.x + bounds.w / 2 && mouse_pos.x < bounds.x + bounds.w) {
                        inner_bound.x = bounds.x + bounds.w / 2;
                        inner_bound.y = bounds.y;
                        inner_bound.w = bounds.w / 2;
                        inner_bound.h = bounds.h;
                    }
                }
                else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                    if (mouse_pos.y >= bounds.y && mouse_pos.y < bounds.y + bounds.h / 2) {
                        inner_bound.x = bounds.x;
                        inner_bound.y = bounds.y;
                        inner_bound.w = bounds.w;
                        inner_bound.h = bounds.h / 2;
                    }
                    else if (mouse_pos.y >= bounds.y + bounds.h / 2 && mouse_pos.y < bounds.y + bounds.h) {
                        inner_bound.x = bounds.x;
                        inner_bound.y = bounds.y + bounds.h / 2;
                        inner_bound.w = bounds.w;
                        inner_bound.h = bounds.h / 2;
                    }
                }
                return inner_bound;
            }
        }
        dck = dck->next;
    }
    return inner_bound;
}

NK_API struct nk_rect
nk_dockspace_dock_highlight(struct nk_context* ctx, nk_flags flag)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    struct nk_rect bound = { 0, 0, 0, 0 };
    if (ctx->dck.flags & NK_DOCK_FINAL) {
        return bound;
    }
    struct nk_dockspace* dck = nk_dockspace_find_window(ctx, ctx->current);
    if (dck && dck->flags & NK_DOCK_LOCK)
        return bound;
    float min_size = 60;

    const struct nk_rect dockable = ctx->dockable_space;
    float set_width = dockable.w / 5;
    set_width = set_width < min_size ? min_size : set_width;
    float set_height = dockable.h / 5;
    set_height = set_height < min_size ? min_size : set_height;

    if (flag == NK_DOCK_TOP && dockable.h > min_size * 2) {
        bound.x = dockable.x + (dockable.w / 2) - (set_width / 2);
        bound.y = dockable.y;
        bound.w = set_width;
        bound.h = set_height;
    }
    else if (flag == NK_DOCK_LEFT && dockable.w > min_size * 2) {
        bound.x = dockable.x;
        bound.y = dockable.y + (dockable.h / 2) - (set_height / 2);
        bound.w = set_width;
        bound.h = set_height;
    }
    else if (flag == NK_DOCK_RIGHT && dockable.w > min_size * 2) {
        bound.x = dockable.x + (dockable.w) - (set_width);
        bound.y = dockable.y + (dockable.h / 2) - (set_height / 2);
        bound.w = set_width;
        bound.h = set_height;
    }
    else if (flag == NK_DOCK_BOTTOM && dockable.h > min_size * 2) {
        bound.x = dockable.x + (dockable.w / 2) - (set_width / 2);
        bound.y = dockable.y + (dockable.h) - (set_height);
        bound.w = set_width;
        bound.h = set_height;
    }

    return bound;
}

NK_API void
nk_dockspace_set_count(struct nk_context* ctx, struct nk_dockspace* dck)
{
    NK_ASSERT(ctx);
    if (!dck) return;

    nk_uint count = 0;
    struct nk_dockspace* cursor = dck->next;
    count++;
    if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_LEFT) {
        while (cursor && cursor->num_windows != 0) {
            if (dck->flags & NK_DOCK_TOP) {
                if (cursor->flags & NK_DOCK_TOP || cursor->flags & NK_DOCK_BOTTOM)
                    count++;
            }
            else if (dck->flags & NK_DOCK_LEFT) {
                if (cursor->flags & NK_DOCK_LEFT || cursor->flags & NK_DOCK_RIGHT)
                    count++;
            }
            cursor = cursor->next;
        }
    }
    else {
        while (cursor && cursor->num_windows != 0) {
            if (dck->flags & NK_DOCK_RIGHT) {
                if (cursor->flags & NK_DOCK_LEFT || cursor->flags & NK_DOCK_RIGHT)
                    count++;
            }
            else if (dck->flags & NK_DOCK_BOTTOM) {
                if (cursor->flags & NK_DOCK_TOP || cursor->flags & NK_DOCK_BOTTOM)
                    count++;
            }
            cursor = cursor->next;
        }
    }
    dck->count = count;
}

NK_API void
nk_dockspace_set(struct nk_context* ctx, struct nk_window* win, nk_flags flag)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (win == 0)
        return;

    if (win->flags & NK_WINDOW_DOCK_SUPER) {
        struct nk_dockspace* dck = nk_dockspace_find_window(ctx, win);
        if (dck) {
            nk_dockspace_relocate(ctx, dck, flag);
            return;
        }
    }

    nk_dockspace_remove_window(ctx, win);
    struct nk_dockspace* dck = &ctx->dck;
    while (dck->num_windows != 0) {
        dck = dck->next;
    }

    const struct nk_rect dockable = ctx->dockable_space;
    if (dck->windows == 0) {
        dck->max_windows = 5;
        dck->windows = (struct nk_docked_window*)malloc(dck->max_windows * sizeof(struct nk_docked_window));
        dck->windows[0].win = win;
        dck->windows[0].originalWidth = win->bounds.w;
        dck->windows[0].originalHeight = win->bounds.h;
    }
    else {
        return;
    }

    if (flag & NK_DOCK_TOP) {
        dck->flags = NK_DOCK_TOP;

        win->bounds.x = dockable.x;
        win->bounds.y = dockable.y;
        win->bounds.w = dockable.w;
        win->bounds.h = win->bounds.h > ctx->dockable_space.h ? ctx->dockable_space.h : win->bounds.h;
        dck->bounds = win->bounds;

        ctx->dockable_space.y += win->bounds.h;
        ctx->dockable_space.h -= win->bounds.h;
    }
    else if (flag & NK_DOCK_LEFT) {
        dck->flags = NK_DOCK_LEFT;

        win->bounds.x = dockable.x;
        win->bounds.y = dockable.y;
        win->bounds.h = dockable.h;
        win->bounds.w = win->bounds.w > ctx->dockable_space.w ? ctx->dockable_space.w : win->bounds.w;
        dck->bounds = win->bounds;

        ctx->dockable_space.x += win->bounds.w;
        ctx->dockable_space.w -= win->bounds.w;
    }
    else if (flag & NK_DOCK_RIGHT) {
        dck->flags = NK_DOCK_RIGHT;

        win->bounds.w = win->bounds.w > ctx->dockable_space.w ? ctx->dockable_space.w : win->bounds.w;
        win->bounds.x = dockable.w - win->bounds.w;
        win->bounds.y = dockable.y;
        win->bounds.h = dockable.h;
        dck->bounds = win->bounds;

        ctx->dockable_space.w -= win->bounds.w;
    }
    else if (flag & NK_DOCK_BOTTOM) {
        dck->flags = NK_DOCK_BOTTOM;

        win->bounds.h = win->bounds.h > ctx->dockable_space.h ? ctx->dockable_space.h : win->bounds.h;
        win->bounds.x = dockable.x;
        win->bounds.y = dockable.h - win->bounds.h;
        win->bounds.w = dockable.w;
        dck->bounds = win->bounds;

        ctx->dockable_space.h -= win->bounds.h;
    }
    if (win->flags & NK_WINDOW_DOCK_SUPER) {
        dck->flags |= NK_DOCK_HAS_SUPER;
    }
    dck->num_windows++;
    dck->count = 0;
    dck->next = (struct nk_dockspace*)malloc(sizeof(struct nk_dockspace));
    dck->next->windows = 0;
    dck->next->num_windows = 0;
    dck->next->flags = 0;
    dck->next->next = 0;
    dck->next->count = 0;

    nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
}

NK_API void
nk_dockspace_append_window(struct nk_context* ctx, struct nk_dockspace* dck, struct nk_window* win, struct nk_rect inner_bound)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);

    if (!win)
        return;

    nk_dockspace_remove_window(ctx, win);
    if (!dck) {
        win->bounds.x = win->bounds.x < 0 ? 0 : win->bounds.x;
        win->bounds.y = win->bounds.y < 0 ? 0 : win->bounds.y;
        nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
        return;
    }

    if (dck->num_windows == dck->max_windows) {
        dck->max_windows += 5;
        dck->windows = (struct nk_docked_window*)realloc(dck->windows, dck->max_windows * sizeof(struct nk_docked_window));
    }

    for (unsigned int i = 0; i < dck->num_windows; i++) {
        if (dck->flags & NK_DOCK_HAS_SUPER && dck->num_windows == 1) {
            struct nk_docked_window newWindow;
            newWindow.win = win;
            newWindow.originalWidth = win->bounds.w;
            newWindow.originalHeight = win->bounds.h;

            win->bounds.x = inner_bound.x;
            win->bounds.y = inner_bound.y;
            if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                win->bounds.w = inner_bound.w;
                win->bounds.h = dck->bounds.h;
                dck->windows[0].win->bounds.w -= win->bounds.w;
                dck->num_windows++;
            }
            else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                win->bounds.w = dck->bounds.w;
                win->bounds.h = inner_bound.h;
                dck->windows[0].win->bounds.h -= win->bounds.h;
                dck->num_windows++;
            }
            dck->windows[1] = newWindow;
            break;
        }
        else if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
            if (inner_bound.x == dck->windows[i].win->bounds.x) {
                struct nk_docked_window newWindow;
                newWindow.win = win;
                newWindow.originalWidth = win->bounds.w;
                newWindow.originalHeight = win->bounds.h;

                win->bounds.x = dck->windows[i].win->bounds.x;
                win->bounds.y = dck->windows[i].win->bounds.y;
                win->bounds.h = dck->windows[i].win->bounds.h;

                dck->windows[i].win->bounds.x += win->bounds.w;
                dck->windows[i].win->bounds.w -= win->bounds.w;

                for (unsigned int j = dck->num_windows; j > i; j--) {
                    struct nk_docked_window temp = dck->windows[j - 1];
                    dck->windows[j] = temp;
                }
                dck->windows[i] = newWindow;
                dck->num_windows++;
                break;
            }
            else if (inner_bound.x == dck->windows[i].win->bounds.x + dck->windows[i].win->bounds.w / 2) {
                struct nk_docked_window newWindow;
                newWindow.win = win;
                newWindow.originalWidth = win->bounds.w;
                newWindow.originalHeight = win->bounds.h;

                win->bounds.x = dck->windows[i].win->bounds.x + dck->windows[i].win->bounds.w - win->bounds.w;
                win->bounds.y = dck->windows[i].win->bounds.y;
                win->bounds.h = dck->windows[i].win->bounds.h;

                dck->windows[i].win->bounds.w -= win->bounds.w;
                i += 1;
                for (unsigned int j = dck->num_windows; j > i; j--) {
                    struct nk_docked_window temp = dck->windows[j - 1];
                    dck->windows[j] = temp;
                }
                dck->windows[i] = newWindow;
                dck->num_windows++;
                break;
            }
        }
        else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
            if (inner_bound.y == dck->windows[i].win->bounds.y) {
                struct nk_docked_window newWindow;
                newWindow.win = win;
                newWindow.originalWidth = win->bounds.w;
                newWindow.originalHeight = win->bounds.h;

                win->bounds.x = dck->windows[i].win->bounds.x;
                win->bounds.y = dck->windows[i].win->bounds.y;
                win->bounds.w = dck->windows[i].win->bounds.w;
                dck->windows[i].win->bounds.y += win->bounds.h;
                dck->windows[i].win->bounds.y -= win->bounds.h;

                for (unsigned int j = dck->num_windows; j > i; j--) {
                    dck->windows[j] = dck->windows[j - 1];
                }
                dck->windows[i] = newWindow;
                dck->num_windows++;
                break;
            }
            else if (inner_bound.y == dck->windows[i].win->bounds.y + dck->windows[i].win->bounds.h / 2) {
                struct nk_docked_window newWindow;
                newWindow.win = win;
                newWindow.originalWidth = win->bounds.w;
                newWindow.originalHeight = win->bounds.h;

                win->bounds.x = dck->windows[i].win->bounds.x;
                win->bounds.y = dck->windows[i].win->bounds.y + dck->windows[i].win->bounds.h - win->bounds.h;
                win->bounds.w = dck->windows[i].win->bounds.w;

                dck->windows[i].win->bounds.h -= win->bounds.h;
                i += 1;
                for (unsigned int j = dck->num_windows; j > i; j--) {
                    dck->windows[j] = dck->windows[j - 1];
                }
                dck->windows[i] = newWindow;
                dck->num_windows++;
                break;
            }
        }
    }
    if (win->flags & NK_WINDOW_DOCK_SUPER) {
        dck->flags |= NK_DOCK_HAS_SUPER;
    }
}

NK_API void
nk_dockspace_assign_window(struct nk_context* ctx, struct nk_dockspace* dck, struct nk_window* win, struct nk_rect inner_bound)
{
    if (dck == 0)
        return;

    if (win->flags & NK_WINDOW_DOCK_SUPER) {
        nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
        return;
    }

    const struct nk_vec2 mouse_pos = ctx->input.mouse.pos;
    if (mouse_pos.x > dck->bounds.x && mouse_pos.x < dck->bounds.x + dck->bounds.w
        && mouse_pos.y > dck->bounds.y && mouse_pos.y < dck->bounds.y + dck->bounds.h) {
        float width = 0.0f;
        float height = 0.0f;
        nk_bool swap_pos = nk_false;
        if ((int)inner_bound.w != 0 && (int)inner_bound.h != 0)
            swap_pos = nk_true;

        unsigned int i = 0;
        for (; i < dck->num_windows; i++) {
            if (dck->windows[i].win == win)
                break;
            if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                width += dck->windows[i].win->bounds.w;
            }
            else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                height += dck->windows[i].win->bounds.h;
            }
        }
        if (!swap_pos) {
            win->bounds.x = dck->bounds.x + width;
            win->bounds.y = dck->bounds.y + height;
            return;
        }

        unsigned int j = 0;
        for (; j < dck->num_windows; j++) {
            if (dck->flags & NK_DOCK_TOP || dck->flags & NK_DOCK_BOTTOM) {
                if (inner_bound.x >= dck->windows[j].win->bounds.x &&
                    inner_bound.x < dck->windows[j].win->bounds.x + dck->windows[j].win->bounds.w)
                    break;
            }
            else if (dck->flags & NK_DOCK_LEFT || dck->flags & NK_DOCK_RIGHT) {
                if (inner_bound.y >= dck->windows[j].win->bounds.y &&
                    inner_bound.y < dck->windows[j].win->bounds.y + dck->windows[j].win->bounds.h)
                    break;
            }
        }
        struct nk_docked_window temp = dck->windows[j];
        dck->windows[j].win = dck->windows[i].win;
        dck->windows[j].originalWidth = dck->windows[i].originalWidth;
        dck->windows[j].originalHeight = dck->windows[i].originalHeight;

        dck->windows[i].win = temp.win;
        dck->windows[i].originalWidth = temp.originalWidth;
        dck->windows[i].originalHeight = temp.originalHeight;
    }
    else {
        nk_dockspace_remove_window(ctx, win);
        win->bounds.x = win->bounds.x < 0 ? 0 : win->bounds.x;
        win->bounds.y = win->bounds.y < 0 ? 0 : win->bounds.y;
    }
    nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
}

NK_API struct nk_dockspace*
nk_dockspace_find_window(struct nk_context* ctx, struct nk_window* win)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);

    struct nk_dockspace* dck = &ctx->dck;
    while (dck != 0) {
        for (unsigned int i = 0; i < dck->num_windows; i++) {
            if (dck->windows[i].win == win) {
                return dck;
            }
        }
        dck = dck->next;
    }
    return 0;
}

NK_API void
nk_dockspace_scale_horizontally(struct nk_context *ctx, struct nk_window *win, float delta)
{
    struct nk_dockspace *dck = nk_dockspace_find_window(ctx, win);
    if(dck != 0) {
        if (dck->windows->win != win) {
            if(dck->flags & NK_DOCK_LEFT) {
                dck->windows->win->bounds.w += delta;
            }
            if (dck->flags & NK_DOCK_RIGHT) {
                dck->windows->win->bounds.x += delta;
                dck->windows->win->bounds.w -= delta;
            }
        }
    }
    nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
}

NK_API void
nk_dockspace_scale_vertically(struct nk_context *ctx, struct nk_window *win, float delta)
{
    struct nk_dockspace *dck = nk_dockspace_find_window(ctx, win);
    if(dck != 0) {
        if (dck->windows->win != win) {
            if(dck->flags & NK_DOCK_TOP) {
                dck->windows->win->bounds.h += delta;
            }
            if (dck->flags & NK_DOCK_BOTTOM) {
                dck->windows->win->bounds.y += delta;
                dck->windows->win->bounds.h -= delta;
            }
        }
    }
    nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
}

NK_API void
nk_dockspace_restore_window(struct nk_context* ctx, struct nk_window* win)
{
    NK_ASSERT(ctx);
    if (!win) return;

    FILE* file = fopen("nuklear.ini", "rb");
    if (!file)
        return;

    char header[64];
    while (fscanf(file, "[%[^]]]\n", header) != EOF) {
        if (!nk_stricmp(header, "Window")) {
            char title[64];
            int x, y, w, h;
            if (fscanf(file, "name=%s\nPos=%d,%d\nWidth=%d\nHeight=%d\n\n", title, &x, &y, &w, &h) != EOF) {
                if (!nk_stricmp(win->name_string, title)) {
                    win->bounds.x = (float)x;
                    win->bounds.y = (float)y;
                    win->bounds.w = (float)w;
                    win->bounds.h = (float)h;
                    break;
                }
            }
        }
        else {
            return;
        }
    }
    fclose(file);
}

NK_API void
nk_dockspace_restore(struct nk_context* ctx)
{
    NK_ASSERT(ctx);
    if (ctx->dock_restored == nk_true)
        return;

    FILE* file = fopen("nuklear.ini", "rb");
    if (!file)
        return;

    int target = 0;
    struct nk_dockspace* dck = &ctx->dck;
    char header[64];
    while (fscanf(file, "[%[^]]]\n", header) != EOF) {
        if (!nk_stricmp(header, "Window")) {
            char title[64];
            int x, y, w, h;
            if (fscanf(file, "name=%s\nPos=%d,%d\nWidth=%d\nHeight=%d\n\n", title, &x, &y, &w, &h) == EOF)
                continue;
        }
        else if (!nk_stricmp(header, "Dockspace")) {
            int index, flags, num_windows, count, max_windows;
            int x, y, w, h;
            if (fscanf(file, "index=%d\nflags=%d\nnum=%d\ncount=%d\nmax=%d\nbounds=%d,%d,%d,%d\n\n",
                &index, &flags, &num_windows, &count, &max_windows, &x, &y, &w, &h) != EOF) {
                if (index == target) {
                    dck->flags = flags;
                    dck->num_windows = num_windows;
                    dck->max_windows = max_windows;
                    dck->count = count;
                    dck->bounds.x = (float)x;
                    dck->bounds.y = (float)y;
                    dck->bounds.w = (float)w;
                    dck->bounds.h = (float)h;
                    dck->windows = (struct nk_docked_window*)malloc(max_windows * sizeof(struct nk_docked_window));
                    target++;

                    char subheader[64];
                    int track = 0;
                    int found_windows = 0;
                    while (fscanf(file, "[%[^]]]\n", subheader) != EOF) {
                        if (!nk_stricmp(subheader, "Dock_Window")) {
                            int pos;
                            char title[64];
                            int width, height;
                            if (fscanf(file, "index=%d\nname=%s\nwidth=%d\nheight=%d\n\n", &pos, title, &width, &height) != EOF) {
                                struct nk_window* iter = ctx->begin;
                                while (iter) {
                                    if (!nk_stricmp(iter->name_string, title)) {
                                        dck->windows[found_windows].win = iter;
                                        dck->windows[found_windows].originalWidth = (float)width;
                                        dck->windows[found_windows].originalHeight = (float)height;
                                        found_windows++;
                                        break;
                                    }
                                    iter = iter->next;
                                }
                                track++;
                                if (track == num_windows)
                                    break;
                            }
                        }
                    }
                    if (found_windows == 0) {
                        dck->flags = 0;
                        dck->num_windows = 0;
                        dck->count = 0;
                        dck->max_windows = 0;
                        free(dck->windows);
                        dck->windows = 0;
                        continue;
                    }

                    dck->num_windows = found_windows;
                    if (dck->windows[0].win->flags & NK_WINDOW_DOCK_SUPER) {
                        dck->flags |= NK_DOCK_HAS_SUPER;
                    }
                    else {
                        dck->flags &= ~NK_DOCK_HAS_SUPER;
                    }
                    dck->next = (struct nk_dockspace*)malloc(sizeof(struct nk_dockspace));
                    dck->next->windows = 0;
                    dck->next->count = 0;
                    dck->next->num_windows = 0;
                    dck->next->flags = 0;
                    dck->next->next = 0;
                    dck = dck->next;
                }
            }
        }
    }
    nk_dockspace_adjust(ctx, ctx->display_size.x, ctx->display_size.y);
    fclose(file);
    ctx->dock_restored = nk_true;
}

NK_API void
nk_dockspace_end(struct nk_context* ctx)
{
    NK_ASSERT(ctx);

    FILE* file = fopen("nuklear.ini", "wb");
    if (!file)
        return;

    struct nk_window* iter = ctx->begin;
    while (iter) {
        fprintf(file, "[Window]\nname=%s\nPos=%d,%d\nWidth=%d\nHeight=%d\n\n",
            iter->name_string,
            (int)iter->bounds.x, (int)iter->bounds.y,
            (int)iter->bounds.w, (int)iter->bounds.h);
        iter = iter->next;
    }
    struct nk_dockspace* dck = &ctx->dck;
    int i = 0;
    while (dck->num_windows != 0) {
        fprintf(file, "[Dockspace]\nindex=%d\nflags=%d\nnum=%d\ncount=%d\nmax=%d\nbounds=%d,%d,%d,%d\n\n",
            i, dck->flags, dck->num_windows, dck->count, dck->max_windows,
            (int)dck->bounds.x, (int)dck->bounds.y, (int)dck->bounds.w, (int)dck->bounds.h);
        for (unsigned int j = 0; j < dck->num_windows; j++) {
            fprintf(file, "[Dock_Window]\nindex=%d\nname=%s\nwidth=%d\nheight=%d\n\n", j,
                dck->windows[j].win->name_string,
                (int)dck->windows[j].originalWidth, (int)dck->windows[j].originalHeight);
        }
        dck = dck->next;
        i++;
    }
    fclose(file);
}

NK_API void
nk_dockspace_lock(struct nk_context* ctx, struct nk_dockspace* dck)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    if (!dck)
        dck = nk_dockspace_find_window(ctx, ctx->current);
    if (!dck) return;

    dck->flags |= NK_DOCK_LOCK;
}

NK_API void
nk_dockspace_unlock(struct nk_context* ctx, struct nk_dockspace* dck)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    if (!dck)
        dck = nk_dockspace_find_window(ctx, ctx->current);
    if (!dck) return;

    dck->flags &= ~NK_DOCK_LOCK;
}


NK_API void
nk_dockspace_make_final(struct nk_context* ctx, nk_bool final)
{
    NK_ASSERT(ctx);
    struct nk_dockspace* dck = &ctx->dck;
    if (final) {
        ctx->dck.flags |= NK_DOCK_FINAL;
        while (dck && dck->num_windows != 0) {
            dck->flags |= NK_DOCK_LOCK;
            dck = dck->next;
        }
    }
    else {
        ctx->dck.flags &= ~NK_DOCK_FINAL;
        while (dck && dck->num_windows != 0) {
            dck->flags &= ~NK_DOCK_LOCK;
            dck = dck->next;
        }
    }
}
