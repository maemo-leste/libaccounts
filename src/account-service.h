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

#ifndef _ACCOUNT_SERVICE_H_
#define _ACCOUNT_SERVICE_H_

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define ACCOUNT_TYPE_SERVICE             (account_service_get_type ())
#define ACCOUNT_SERVICE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_SERVICE, AccountService))
#define ACCOUNT_SERVICE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_SERVICE, AccountServiceClass))
#define ACCOUNT_IS_SERVICE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_SERVICE))
#define ACCOUNT_IS_SERVICE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_SERVICE))
#define ACCOUNT_SERVICE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_SERVICE, AccountServiceClass))

typedef struct _AccountServiceClass AccountServiceClass;
typedef struct _AccountService AccountService;

#include "account-plugin.h"
#include "account-edit-context.h"

struct _AccountServiceClass
{
    GObjectClass parent_class;
};

struct _AccountService
{
    GObject parent_instance;

    /*< protected >*/
    gchar *name;
    gchar *display_name;
    gboolean supports_avatar;
    GdkPixbuf *icon;
    AccountPlugin *plugin;
    gint priority;
    gchar *service_name;
};

GType account_service_get_type (void) G_GNUC_CONST;

AccountEditContext *account_service_begin_new (AccountService *service);
AccountPlugin *account_service_get_plugin (AccountService *service);

const gchar *account_service_get_name (AccountService *service);
const gchar *account_service_get_display_name (AccountService *service);
gint account_service_get_priority (AccountService *service);

G_END_DECLS

#endif /* _ACCOUNT_SERVICE_H_ */
