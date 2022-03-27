/* vi: set et sw=4 ts=4 cino=t0,(0: */
/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of osso-accounts
 *
 * Copyright (C) 2007 Nokia Corporation. All rights reserved.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@nokia.com>
 * Contact: Naba Kumar <naba.kumar@nokia.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating, any or all
 * of this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */

#ifndef _ACCOUNT_WIZARD_CONTEXT_H_
#define _ACCOUNT_WIZARD_CONTEXT_H_

#include <glib-object.h>
#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define ACCOUNT_TYPE_WIZARD_CONTEXT             (account_wizard_context_get_type ())
#define ACCOUNT_WIZARD_CONTEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_WIZARD_CONTEXT, AccountWizardContext))
#define ACCOUNT_IS_WIZARD_CONTEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_WIZARD_CONTEXT))
#define ACCOUNT_WIZARD_CONTEXT_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ACCOUNT_TYPE_WIZARD_CONTEXT, AccountWizardContextIface))

typedef struct _AccountWizardContextIface AccountWizardContextIface;
typedef struct _AccountWizardContext AccountWizardContext;

struct _AccountWizardContextIface
{
    GTypeInterface g_iface;

    /* methods */
    GtkWidget * (*next) (AccountWizardContext *context, GError **error);
    GtkWidget * (*back) (AccountWizardContext *context, GError **error);
    const gchar * (*get_page_title) (AccountWizardContext *context);

    gboolean (*finish) (AccountWizardContext *context, GError **error);
    void (*cancel) (AccountWizardContext *context);
};

GType account_wizard_context_get_type (void) G_GNUC_CONST;

gboolean account_wizard_context_get_can_next (AccountWizardContext *context);
gboolean account_wizard_context_get_can_back (AccountWizardContext *context);
gboolean account_wizard_context_get_can_finish (AccountWizardContext *context);

void account_wizard_context_set_can_next (AccountWizardContext *context, gboolean can_next);
void account_wizard_context_set_can_back (AccountWizardContext *context, gboolean can_back);
void account_wizard_context_set_can_finish (AccountWizardContext *context, gboolean can_finish);

GtkWidget *account_wizard_context_next (AccountWizardContext *context, GError **error);
GtkWidget *account_wizard_context_back (AccountWizardContext *context, GError **error);
const gchar *account_wizard_context_get_page_title (AccountWizardContext *context);

gboolean account_wizard_context_finish (AccountWizardContext *context, GError **error);
void account_wizard_context_cancel (AccountWizardContext *context);

G_END_DECLS

#endif /* _ACCOUNT_WIZARD_CONTEXT_H_ */
