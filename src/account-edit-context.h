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

#ifndef _ACCOUNT_EDIT_CONTEXT_H_
#define _ACCOUNT_EDIT_CONTEXT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define ACCOUNT_TYPE_EDIT_CONTEXT             (account_edit_context_get_type ())
#define ACCOUNT_EDIT_CONTEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_EDIT_CONTEXT, AccountEditContext))
#define ACCOUNT_EDIT_CONTEXT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_EDIT_CONTEXT, AccountEditContextClass))
#define ACCOUNT_IS_EDIT_CONTEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_EDIT_CONTEXT))
#define ACCOUNT_IS_EDIT_CONTEXT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_EDIT_CONTEXT))
#define ACCOUNT_EDIT_CONTEXT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_EDIT_CONTEXT, AccountEditContextClass))

typedef struct _AccountEditContextClass AccountEditContextClass;
typedef struct _AccountEditContext AccountEditContext;

#include "account-item.h"

struct _AccountEditContextClass
{
    GObjectClass parent_class;
};

struct _AccountEditContext
{
    GObject parent_instance;
};

GType account_edit_context_get_type (void) G_GNUC_CONST;

AccountItem *account_edit_context_get_account (AccountEditContext *context);
AccountPlugin *account_edit_context_get_plugin (AccountEditContext *context);

gboolean account_edit_context_get_editing (AccountEditContext *context);

G_END_DECLS

#endif /* _ACCOUNT_EDIT_CONTEXT_H_ */
