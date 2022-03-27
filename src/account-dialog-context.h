/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of osso-accounts
 *
 * Copyright (C) 2008 Nokia Corporation. All rights reserved.
 *
 * Contact: Salvatore Iovene <ext-salvatore.iovene@nokia.com>
 * Contact: Naba Kumar <naba.kumar@nokia.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating, any or all
 * of this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */

#ifndef _ACCOUNT_DIALOG_CONTEXT_H_
#define _ACCOUNT_DIALOG_CONTEXT_H_

#include <glib-object.h>
#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define ACCOUNT_TYPE_DIALOG_CONTEXT             (account_dialog_context_get_type ())
#define ACCOUNT_DIALOG_CONTEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_DIALOG_CONTEXT, AccountDialogContext))
#define ACCOUNT_IS_DIALOG_CONTEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_DIALOG_CONTEXT))
#define ACCOUNT_DIALOG_CONTEXT_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ACCOUNT_TYPE_DIALOG_CONTEXT, AccountDialogContextIface))

typedef struct _AccountDialogContextIface AccountDialogContextIface;
typedef struct _AccountDialogContext AccountDialogContext;

struct _AccountDialogContextIface
{
    GTypeInterface g_iface;

    /* methods */
    GtkWidget * (*start) (AccountDialogContext *context, GError **error);
    gboolean (*finish) (AccountDialogContext *context, GError **error);
    const gchar * (*get_page_title) (AccountDialogContext *context);

    void (*cancel) (AccountDialogContext *context);
};

#define RESPONSE_DONE 1

GType account_dialog_context_get_type (void) G_GNUC_CONST;

GtkWidget *account_dialog_context_start (AccountDialogContext *context, GError **error);
const gchar *account_dialog_context_get_page_title (AccountDialogContext *context);

gboolean account_dialog_context_finish (AccountDialogContext *context, GError **error);
void account_dialog_context_cancel (AccountDialogContext *context);

G_END_DECLS

#endif /* _ACCOUNT_DIALOG_CONTEXT_H_ */
