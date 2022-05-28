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

#ifndef _ACCOUNT_PLUGIN_H_
#define _ACCOUNT_PLUGIN_H_

#include <glib-object.h>
#include "account-plugin-loader.h"

G_BEGIN_DECLS

#define ACCOUNT_TYPE_PLUGIN             (account_plugin_get_type ())
#define ACCOUNT_PLUGIN(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNT_TYPE_PLUGIN, AccountPlugin))
#define ACCOUNT_PLUGIN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ACCOUNT_TYPE_PLUGIN, AccountPluginClass))
#define ACCOUNT_IS_PLUGIN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNT_TYPE_PLUGIN))
#define ACCOUNT_IS_PLUGIN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ACCOUNT_TYPE_PLUGIN))
#define ACCOUNT_PLUGIN_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ACCOUNT_TYPE_PLUGIN, AccountPluginClass))

typedef struct _AccountPluginClass AccountPluginClass;
typedef struct _AccountPlugin AccountPlugin;

#include "accounts-list.h"
#include "account-service.h"
#include "account-edit-context.h"

struct _AccountPluginClass
{
    GObjectClass parent_class;

    /* Virtual methods */
    gboolean (*setup) (AccountPlugin *plugin, AccountsList *accounts_list);
    
    const gchar* (*get_name) (AccountPlugin *plugin);
    const gchar* (*get_display_name) (AccountPlugin *plugin);
    
    AccountEditContext* (*begin_new) (AccountPlugin *plugin,
                                      AccountService *service);
    AccountEditContext* (*begin_edit) (AccountPlugin *plugin,
                                       AccountItem *account_item);
    GList* (*list_services) (AccountPlugin *plugin);
    void (*deleted) (AccountPlugin *plugin, AccountItem *account_item);
};

struct _AccountPlugin
{
    GObject parent_instance;
};

GType account_plugin_get_type (void) G_GNUC_CONST;

#define ACCOUNT_DEFINE_PLUGIN(TN, t_n, T_P) \
ACCOUNT_DEFINE_TYPE_MODULE_EXTENDED(TN, t_n, T_P, 0, {})

#define ACCOUNT_DEFINE_PLUGIN_WITH_PRIVATE(TN, t_n, T_P) \
ACCOUNT_DEFINE_TYPE_MODULE_EXTENDED(TN, t_n, T_P, 0, G_ADD_PRIVATE_DYNAMIC(TN))

#define ACCOUNT_DEFINE_TYPE_MODULE_EXTENDED(TN, t_n, T_P, flags, CODE) \
G_DEFINE_DYNAMIC_TYPE_EXTENDED(TN, t_n, T_P, flags, CODE)              \
ACCOUNT_PLUGIN_SYMBOLS(TN, t_n)

#define ACCOUNT_PLUGIN_SYMBOLS(TN, t_n)                                  \
G_MODULE_EXPORT void account_plugin_load(AccountPluginLoader *plugin);   \
void account_plugin_load(AccountPluginLoader *plugin)                    \
{                                                                        \
  if (G_UNLIKELY(t_n##_type_id == 0))                                    \
    t_n##_register_type(G_TYPE_MODULE(plugin));                          \
  account_plugin_loader_add_type(plugin, t_n##_type_id);                 \
}                                                                        \
G_MODULE_EXPORT void account_plugin_unload(AccountPluginLoader *plugin); \
void account_plugin_unload(AccountPluginLoader *plugin)                  \
{                                                                        \
}                                                                        \
static void t_n##_class_finalize(TN##Class *klass)                       \
{                                                                        \
}

gboolean account_plugin_setup (AccountPlugin *plugin,
                               AccountsList *accounts_list);

const gchar *account_plugin_get_name (AccountPlugin *plugin);
const gchar *account_plugin_get_display_name (AccountPlugin *plugin);

GList *account_plugin_list_services (AccountPlugin *plugin);

AccountEditContext *account_plugin_begin_new (AccountPlugin *plugin,
                                              AccountService *service);
AccountEditContext *account_plugin_begin_edit (AccountPlugin *plugin,
                                               AccountItem *account_item);

G_END_DECLS

#endif /* _ACCOUNT_PLUGIN_H_ */
