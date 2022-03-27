/*
 * account-plugin.c
 *
 * Copyright (C) 2022 Ivaylo Dimitrov <ivo.g.dimitrov.75@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * SECTION:account-plugin
 * @short_description: manage account items.
 *
 * An #AccountPlugin is an object used to create and manage accounts. Use the
 * account_plugin_begin_new() method to create an #AccountItem for accessing the
 * given #AccountService (see account_plugin_list_services() for knowing what
 * services are provided by the plugin) and account_plugin_begin_edit() to edit
 * an existing #AccountItem. Both methods return an #AccountEditContext object,
 * which provides methods for performing the actual editing (so far a
 * wizard-like interface has been implemented, #AccountWizardContext).
 *
 * Account plugins report about the accounts they know of to an #AccountsList
 * object (actually, #AccountsList is an interface), which is typically the user
 * interface, and which will be bound to the account when the
 * #AccountPluginManager will setup the plugin (by calling
 * account_plugin_setup()).
 *
 * Plugin implementations must derive from #AccountPlugin (using the
 * %ACCOUNT_DEFINE_PLUGIN() macro, which in fact registers a #GTypeModule) and
 * implement all the virtual methods of the #AccountPluginClass.
 *
 * Plugins are created in the #AccountPluginManager.
 */

#include "config.h"

#include "account-plugin.h"

struct _AccountPluginPrivate
{
  AccountsList *accounts_list;
};

typedef struct _AccountPluginPrivate AccountPluginPrivate;

#define PRIVATE(plugin) \
  ((AccountPluginPrivate *) \
   account_plugin_get_instance_private((AccountPlugin *)(plugin)))

G_DEFINE_TYPE_WITH_PRIVATE(
  AccountPlugin,
  account_plugin,
  G_TYPE_OBJECT
)

enum
{
  PROP_ACCOUNT_LIST = 1,
  PROP_INITIALIZED
};

#define NOT_IMPLEMENTED(fun, parameters, code) \
  _account_plugin_ ## fun parameters \
  { \
    g_warning("AccountPlugin::" # fun "not implemented for `%s'", \
              g_type_name(G_TYPE_FROM_INSTANCE(plugin))); \
    code \
  }

/* *INDENT-OFF* */
static gboolean
NOT_IMPLEMENTED(initialized, (AccountPlugin *plugin),
                return FALSE;)

static void
NOT_IMPLEMENTED(deleted, (AccountPlugin *plugin, AccountItem *account_item), )

static gboolean
NOT_IMPLEMENTED(setup, (AccountPlugin *plugin, AccountsList *accounts_list),
                return FALSE;)

const gchar *
NOT_IMPLEMENTED(get_name, (AccountPlugin *plugin),
                return NULL;)

const gchar *
NOT_IMPLEMENTED(get_display_name, (AccountPlugin *plugin),
                return NULL;)

static AccountEditContext *
NOT_IMPLEMENTED(begin_new, (AccountPlugin *plugin, AccountService *service),
                return NULL;)

static AccountEditContext *
NOT_IMPLEMENTED(begin_edit, (AccountPlugin *plugin, AccountItem *account_item),
                return NULL;)

static GList *
NOT_IMPLEMENTED(list_services, (AccountPlugin *plugin),
                return NULL;)
/* *INDENT-ON* */

static void
account_removed(AccountsList *accounts_list, AccountItem *account_item,
                AccountPlugin *plugin)
{
  if (account_item_get_plugin(account_item) == plugin)
    ACCOUNT_PLUGIN_GET_CLASS(plugin)->deleted(plugin, account_item);
}

static void
account_plugin_dispose(GObject *object)
{
  AccountPluginPrivate *priv = PRIVATE(object);

  if (priv->accounts_list)
  {
    g_signal_handlers_disconnect_matched(
      priv->accounts_list, G_SIGNAL_MATCH_DATA | G_SIGNAL_MATCH_FUNC, 0, 0,
      NULL, account_removed, object);
    priv->accounts_list = NULL;
  }

  G_OBJECT_CLASS(account_plugin_parent_class)->dispose(object);
}

static void
account_plugin_finalize(GObject *object)
{
  G_OBJECT_CLASS(account_plugin_parent_class)->finalize(object);
}

static void
account_plugin_set_property(GObject *object, guint property_id,
                            const GValue *value, GParamSpec *pspec)
{
  g_return_if_fail(ACCOUNT_IS_PLUGIN(object));

  switch (property_id)
  {
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

static void
account_plugin_get_property(GObject *object, guint property_id, GValue *value,
                            GParamSpec *pspec)
{
  AccountPluginPrivate *priv = PRIVATE(object);

  g_return_if_fail(ACCOUNT_IS_PLUGIN(object));

  switch (property_id)
  {
    case PROP_ACCOUNT_LIST:
    {
      g_value_set_object(value, priv->accounts_list);
      break;
    }
    case PROP_INITIALIZED:
    {
      g_value_set_boolean(value,
                          _account_plugin_initialized(ACCOUNT_PLUGIN(object)));
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
    }
  }
}

static void
account_plugin_class_init(AccountPluginClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = account_plugin_dispose;
  object_class->finalize = account_plugin_finalize;
  object_class->set_property = account_plugin_set_property;
  object_class->get_property = account_plugin_get_property;

  klass->setup = _account_plugin_setup;
  klass->get_name = _account_plugin_get_name;
  klass->get_display_name = _account_plugin_get_display_name;
  klass->begin_new = _account_plugin_begin_new;
  klass->begin_edit = _account_plugin_begin_edit;
  klass->list_services = _account_plugin_list_services;
  klass->deleted = _account_plugin_deleted;

  g_object_class_install_property(
    object_class, PROP_ACCOUNT_LIST,
    g_param_spec_object(
      "accounts-list",
      "AccountsList",
      "AccountsList to be notified of account creations/deletions",
      ACCOUNTS_TYPE_LIST,
      G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_INITIALIZED,
    g_param_spec_boolean("initialized",
                         "Initialized",
                         "Whether plugin has been initialized",
                         FALSE,
                         G_PARAM_READABLE));
}

static void
account_plugin_init(AccountPlugin *plugin)
{}

/**
 * account_plugin_setup:
 * @plugin: the AccountPlugin.
 * @accounts_list: an object implementing the #AccountsList interface.
 *
 * Activates the @plugin, and binds it to an #AccountsList object. The plugin
 * will report to it all the #AccountItem object being created/deleted.
 *
 * Returns: %TRUE on success, %FALSE on error.
 */
gboolean
account_plugin_setup(AccountPlugin *plugin, AccountsList *accounts_list)
{
  AccountPluginPrivate *priv;

  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), FALSE);

  priv = PRIVATE(plugin);

  g_return_val_if_fail(priv->accounts_list == NULL, FALSE);

  priv->accounts_list = accounts_list;

  g_signal_connect(accounts_list, "remove-item",
                   G_CALLBACK(account_removed), plugin);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->setup(plugin, accounts_list);
}

/**
 * account_plugin_get_name:
 * @plugin: the #AccountPlugin.
 *
 * Get the name of @plugin.
 *
 * Returns:(transfer none): The name of the @plugin.
 */
const gchar *
account_plugin_get_name(AccountPlugin *plugin)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), NULL);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->get_name(plugin);
}

/**
 * account_plugin_get_display_name:
 * @plugin: the #AccountPlugin.
 *
 * Get the name of @plugin.
 *
 * Returns:(transfer none): The name of the @plugin to be displayed in the user
 * interface.
 */
const gchar *
account_plugin_get_display_name(AccountPlugin *plugin)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), NULL);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->get_display_name(plugin);
}

/**
 * account_plugin_begin_new:
 * @plugin: an #AccountPlugin.
 * @service: an AccountService.
 *
 * Starts the procedure of creation of a new account based on @service.
 *
 * Returns:(transfer full): an #AccountEditContext.
 */
AccountEditContext *
account_plugin_begin_new(AccountPlugin *plugin, AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), NULL);
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), NULL);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->begin_new(plugin, service);
}

/**
 * account_plugin_begin_edit:
 * @plugin: an #AccountPlugin.
 * @account_item: an #AccountItem.
 *
 * Starts the procedure of editing @account.
 *
 * Returns:(transfer full): an #AccountEditContext.
 */
AccountEditContext *
account_plugin_begin_edit(AccountPlugin *plugin, AccountItem *account_item)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), NULL);
  g_return_val_if_fail(ACCOUNT_IS_ITEM(account_item), NULL);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->begin_edit(plugin, account_item);
}

/**
 * account_plugin_list_services:
 * @plugin: the AccountPlugin.
 *
 * List the services handled by this @plugin.
 *
 * Returns:(transfer container): a #GList of #AccountService objects.
 */
GList *
account_plugin_list_services(AccountPlugin *plugin)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN(plugin), NULL);

  return ACCOUNT_PLUGIN_GET_CLASS(plugin)->list_services(plugin);
}
