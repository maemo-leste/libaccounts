/*
 * account-plugin-manager.c
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
 * SECTION:account-plugin-manager
 * @short_description: manage account plugins.
 *
 * The #AccountPluginManager is a convenience object for managing account
 * plugins. When it is first created with account_plugin_manager_new() it scans
 * through the given list of paths, looking for loadable plugins. For each
 * module it finds, it creates an #AccountPlugin object and binds it to the
 * given #AccountsList (by calling account_plugin_setup()).
 *
 * The account_plugin_manager_list() method can be used to retrieve the list of
 * the known #AccountPlugin objects.
 */

#include "config.h"

#include "account-plugin-manager.h"

struct _AccountPluginManagerPrivate
{
  GList *plugin_paths;
  AccountsList *accounts_list;
  GList *plugins;
  guint pending_count;
};

typedef struct _AccountPluginManagerPrivate AccountPluginManagerPrivate;

#define PRIVATE(manager) \
  ((AccountPluginManagerPrivate *) \
   account_plugin_manager_get_instance_private( \
     (AccountPluginManager *)(manager)))

G_DEFINE_TYPE_WITH_PRIVATE(
  AccountPluginManager,
  account_plugin_manager,
  G_TYPE_OBJECT
)

enum
{
  PROP_PLUGIN_PATHS = 1,
  PROP_ACCOUNTS_LIST,
  PROP_PLUGINS_INITIALIZED
};

static void
on_plugin_initialized(AccountPlugin *plugin,
                      GParamSpec *pspec,
                      AccountPluginManager *manager)
{
  AccountPluginManagerPrivate *priv = PRIVATE(manager);
  gboolean initialized;

  g_object_get(G_OBJECT(plugin),
               "initialized", &initialized,
               FALSE);

  if (initialized)
  {
    if (priv->pending_count-- == 1)
      g_object_notify(G_OBJECT(manager), "plugins-initialized");

    g_signal_handlers_disconnect_matched(
      plugin, G_SIGNAL_MATCH_DATA | G_SIGNAL_MATCH_FUNC, 0, 0, NULL,
      on_plugin_initialized, manager);
  }
}

static void
account_plugin_manager_dispose(GObject *object)
{
  AccountPluginManagerPrivate *priv = PRIVATE(object);
  GList *l;

  if (priv->accounts_list)
  {
    g_object_unref(priv->accounts_list);
    priv->accounts_list = NULL;
  }

  for (l = priv->plugins; l; l = l->next)
  {
    g_signal_handlers_disconnect_matched(
      l->data, G_SIGNAL_MATCH_DATA | G_SIGNAL_MATCH_FUNC, 0, 0, NULL,
      on_plugin_initialized, object);
    g_object_unref(l->data);
  }

  if (priv->plugins)
  {
    g_list_free(priv->plugins);
    priv->plugins = NULL;
  }

  G_OBJECT_CLASS(account_plugin_manager_parent_class)->dispose(object);
}

static GList *
list_plugins(AccountPluginManagerPrivate *priv)
{
  GList *l;
  GList *plugins = 0;

  for (l = priv->plugin_paths; l; l = l->next)
  {
    GDir *dir = g_dir_open(l->data, 0, NULL);

    if (dir)
    {
      const gchar *name;

      while ((name = g_dir_read_name(dir)))
      {
        if (g_str_has_suffix(name, ".so"))
        {
          gchar *path = g_build_filename(l->data, name, NULL);
          AccountPluginLoader *loader = account_plugin_loader_new(path);

          g_free(path);

          if (g_type_module_use(G_TYPE_MODULE(loader)))
          {
            plugins = g_list_concat(plugins,
                                    account_plugin_loader_get_objects(loader));
          }
          else
            g_warning("%s: could not load plugin %s", __FUNCTION__, name);
        }
      }

      g_dir_close(dir);
    }
  }

  return plugins;
}

static GObject *
account_plugin_manager_constructor(GType type, guint n_construct_properties,
                                   GObjectConstructParam *construct_properties)
{
  AccountPluginManagerPrivate *priv;
  GList *l;
  GObject *object;

  object = G_OBJECT_CLASS(account_plugin_manager_parent_class)->constructor(
      type, n_construct_properties, construct_properties);

  priv = PRIVATE(object);

  if (!priv->accounts_list)
    return NULL;

  priv->plugins = list_plugins(priv);

  for (l = priv->plugins; l; l = l->next)
  {
    gboolean initialized = FALSE;

    if (!account_plugin_setup(l->data, priv->accounts_list))
    {
      g_warning( "%s: Initialization of plugin %s failed", __FUNCTION__,
                 g_type_name(G_TYPE_FROM_INSTANCE(l->data)));
    }

    g_object_get(l->data, "initialized", &initialized, NULL);

    if (!initialized)
    {
      priv->pending_count++;
      g_signal_connect(l->data, "notify::initialized",
                       G_CALLBACK(on_plugin_initialized), object);
    }
  }

  if (!priv->pending_count)
    g_object_notify(object, "plugins-initialized");

  return object;
}

static void
account_plugin_manager_finalize(GObject *object)
{
  AccountPluginManagerPrivate *priv = PRIVATE(object);

  g_list_free_full(priv->plugin_paths, g_free);
  G_OBJECT_CLASS(account_plugin_manager_parent_class)->finalize(object);
}

static void
add_string(const gchar *path, AccountPluginManagerPrivate *priv)
{
  priv->plugin_paths = g_list_prepend(priv->plugin_paths, g_strdup(path));
}

static void
account_plugin_manager_set_property(GObject *object, guint property_id,
                                    const GValue *value, GParamSpec *pspec)
{
  AccountPluginManagerPrivate *priv;

  g_return_if_fail(ACCOUNT_IS_PLUGIN_MANAGER(object));

  priv = PRIVATE(object);

  switch (property_id)
  {
    case PROP_PLUGIN_PATHS:
    {
      g_list_foreach(g_value_get_pointer(value), (GFunc)add_string, priv);
      break;
    }
    case PROP_ACCOUNTS_LIST:
    {
      priv->accounts_list = g_value_dup_object(value);
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
account_plugin_manager_get_property(GObject *object, guint property_id,
                                    GValue *value, GParamSpec *pspec)
{
  AccountPluginManagerPrivate *priv;

  g_return_if_fail(ACCOUNT_IS_PLUGIN_MANAGER(object));

  priv = PRIVATE(object);

  switch (property_id)
  {
    case PROP_ACCOUNTS_LIST:
    {
      g_value_set_object(value, priv->accounts_list);
      break;
    }
    case PROP_PLUGINS_INITIALIZED:
    {
      g_value_set_boolean(value, priv->pending_count == 0);
      break;
    }
    case PROP_PLUGIN_PATHS:
    {
      g_value_set_pointer(value, priv->plugin_paths);
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
account_plugin_manager_class_init(AccountPluginManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = account_plugin_manager_dispose;
  object_class->finalize = account_plugin_manager_finalize;
  object_class->constructor = account_plugin_manager_constructor;
  object_class->set_property = account_plugin_manager_set_property;
  object_class->get_property = account_plugin_manager_get_property;

  g_object_class_install_property(
    object_class, PROP_PLUGIN_PATHS,
    g_param_spec_pointer(
      "plugin-paths",
      "Plugin paths",
      "Plugin installation paths",
      G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
  g_object_class_install_property(
    object_class, PROP_ACCOUNTS_LIST,
    g_param_spec_object(
      "accounts-list",
      "AccountsList",
      "AccountsList to be notified of account creations/deletions",
      ACCOUNTS_TYPE_LIST,
      G_PARAM_CONSTRUCT_ONLY|G_PARAM_READWRITE));
  g_object_class_install_property(
    object_class, PROP_PLUGINS_INITIALIZED,
    g_param_spec_boolean(
      "plugins-initialized",
      "Plugins initialized",
      "Whether plugins have been initialized",
      FALSE,
      G_PARAM_READABLE));
}

static void
account_plugin_manager_init(AccountPluginManager *manager)
{}

/**
 * account_plugin_manager_list:
 * @plugin_manager: the #AccountPluginManager to get plugins list
 *
 * Returns a list of all the available account plugins.
 *
 * Returns:(transfer container): a #GList of #AccountPlugin objects
 */
GList *
account_plugin_manager_list(AccountPluginManager *plugin_manager)
{
  g_return_val_if_fail(ACCOUNT_IS_PLUGIN_MANAGER(plugin_manager), NULL);

  return g_list_copy(PRIVATE(plugin_manager)->plugins);
}

/**
 * account_plugin_manager_new:
 * @plugin_paths: #GList of plugin paths.
 * @accounts_list: the #AccountsList.
 *
 * Create an #AccountPluginManager for handling the plugins located in any of
 * the directories specified by plugin_paths, and binds @accounts_list to it.
 *
 * Returns:(transfer full): an #AccountPluginManager.
 */
AccountPluginManager *
account_plugin_manager_new(GList *plugin_paths, AccountsList *accounts_list)
{

  g_return_val_if_fail(plugin_paths != NULL, NULL);
  g_return_val_if_fail(accounts_list != NULL, NULL);

  return g_object_new(ACCOUNT_TYPE_PLUGIN_MANAGER,
                      "plugin-paths", plugin_paths,
                      "accounts-list", accounts_list,
                      NULL);
}
