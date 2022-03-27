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

#ifndef _ACCOUNT_ITEM_H_
#define _ACCOUNT_ITEM_H_

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define ACCOUNT_TYPE_ITEM             (account_item_get_type ())
#define ACCOUNT_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_ITEM, AccountItem))
#define ACCOUNT_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_ITEM, AccountItemClass))
#define ACCOUNT_IS_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_ITEM))
#define ACCOUNT_IS_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_ITEM))
#define ACCOUNT_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_ITEM, AccountItemClass))

typedef struct _AccountItemClass AccountItemClass;
typedef struct _AccountItem AccountItem;

#include "account-service.h"

struct _AccountItemClass
{
    GObjectClass parent_class;

    gboolean (*set_enabled) (AccountItem *account, gboolean enabled,
                             GError **error);
};

struct _AccountItem
{
    GObject parent_instance;

    /*< protected >*/
    gchar *name;
    gchar *display_name;
    GdkPixbuf *avatar;
    gchar *service_name;
    GdkPixbuf *service_icon;
    AccountService *service;
    guint enabled : 1;
    guint draft : 1;
    guint connected : 1;
    guint supports_avatar : 1;
};

GType account_item_get_type (void) G_GNUC_CONST;

gboolean account_item_set_enabled (AccountItem *account, gboolean enabled,
                                   GError **error);

AccountPlugin *account_item_get_plugin (AccountItem *account);
AccountService *account_item_get_service (AccountItem *account);
gboolean account_item_is_connected (AccountItem *account) G_GNUC_DEPRECATED;

G_END_DECLS

#endif /* _ACCOUNT_ITEM_H_ */
