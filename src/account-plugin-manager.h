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

#ifndef _ACCOUNT_PLUGIN_MANAGER_H_
#define _ACCOUNT_PLUGIN_MANAGER_H_

#include <glib-object.h>
#include "account-plugin.h"

G_BEGIN_DECLS

#define ACCOUNT_TYPE_PLUGIN_MANAGER             (account_plugin_manager_get_type ())
#define ACCOUNT_PLUGIN_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_PLUGIN_MANAGER, AccountPluginManager))
#define ACCOUNT_PLUGIN_MANAGER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_PLUGIN_MANAGER, AccountPluginManagerClass))
#define ACCOUNT_IS_PLUGIN_MANAGER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_PLUGIN_MANAGER))
#define ACCOUNT_IS_PLUGIN_MANAGER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_PLUGIN_MANAGER))
#define ACCOUNT_PLUGIN_MANAGER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_PLUGIN_MANAGER, AccountPluginManagerClass))

typedef struct _AccountPluginManagerClass AccountPluginManagerClass;
typedef struct _AccountPluginManager AccountPluginManager;

struct _AccountPluginManagerClass
{
    GObjectClass parent_class;
};

struct _AccountPluginManager
{
    GObject parent_instance;
};

GType account_plugin_manager_get_type (void) G_GNUC_CONST;

GList *account_plugin_manager_list (AccountPluginManager *plugin_manager);
AccountPluginManager* account_plugin_manager_new (GList *plugin_paths,
                                                  AccountsList *accounts_list);

G_END_DECLS

#endif /* _ACCOUNT_PLUGIN_MANAGER_H_ */
