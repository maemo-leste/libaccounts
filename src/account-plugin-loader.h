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

#ifndef _ACCOUNT_PLUGIN_LOADER_H_
#define _ACCOUNT_PLUGIN_LOADER_H_

#include <glib-object.h>
#include "account-wizard-context.h"

G_BEGIN_DECLS

#define ACCOUNT_TYPE_PLUGIN_LOADER             (account_plugin_loader_get_type ())
#define ACCOUNT_PLUGIN_LOADER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_PLUGIN_LOADER, AccountPluginLoader))
#define ACCOUNT_PLUGIN_LOADER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_PLUGIN_LOADER, AccountPluginLoaderClass))
#define ACCOUNT_IS_PLUGIN_LOADER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_PLUGIN_LOADER))
#define ACCOUNT_IS_PLUGIN_LOADER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_PLUGIN_LOADER))
#define ACCOUNT_PLUGIN_LOADER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_PLUGIN_LOADER, AccountPluginLoaderClass))

typedef struct _AccountPluginLoaderClass AccountPluginLoaderClass;
typedef struct _AccountPluginLoader AccountPluginLoader;
typedef struct _AccountPluginLoaderPrivate AccountPluginLoaderPrivate;

struct _AccountPluginLoaderClass
{
    GTypeModuleClass parent_class;
};

struct _AccountPluginLoader
{
    GTypeModule parent_instance;

    GList *gtypes;
};

GType account_plugin_loader_get_type (void) G_GNUC_CONST;

AccountPluginLoader *account_plugin_loader_new (const gchar *path);

GList *account_plugin_loader_get_objects (AccountPluginLoader *plugin_loader);
void account_plugin_loader_add_type (AccountPluginLoader *plugin_loader, GType type);



G_END_DECLS

#endif /* _ACCOUNT_PLUGIN_LOADER_H_ */
