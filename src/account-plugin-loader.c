/*
 * account-plugin-loader.c
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

#include "config.h"

#include "account-plugin-loader.h"

/**
 * SECTION:account-plugin-loader
 * @short_description: A representation of an #AccountPlugin module loader.
 *
 * The #AccountPluginLoader objects are used internally by libaccount; the API
 * is public, but you probably won't have to deal directly with it, since the
 * %ACCOUNT_DEFINE_PLUGIN() macro already takes care of this.
 */

struct _AccountPluginLoaderPrivate
{
  /** path of the plugin to load */
  gchar *path;
  /** loaded module */
  GModule *module;
  /** module load function */
  void (*load)(AccountPluginLoader *plugin);
  /** module unload function */
  void (*unload)(AccountPluginLoader *plugin);
};

typedef struct _AccountPluginLoaderPrivate AccountPluginLoaderPrivate;

#define PRIVATE(loader) \
  ((AccountPluginLoaderPrivate *) \
   account_plugin_loader_get_instance_private((AccountPluginLoader *)(loader)))

G_DEFINE_TYPE_WITH_PRIVATE(
  AccountPluginLoader,
  account_plugin_loader,
  G_TYPE_TYPE_MODULE
)

enum
{
  PROP_PATH = 1
};

static void
account_plugin_loader_finalize(GObject *object)
{
  AccountPluginLoader *plugin = ACCOUNT_PLUGIN_LOADER(object);
  AccountPluginLoaderPrivate *priv = PRIVATE(plugin);

  g_list_free(plugin->gtypes);
  g_free(priv->path);

  G_OBJECT_CLASS(account_plugin_loader_parent_class)->finalize(object);
}

static void
account_plugin_loader_set_property(GObject *object, guint property_id,
                                   const GValue *value, GParamSpec *pspec)
{
  AccountPluginLoader *plugin = ACCOUNT_PLUGIN_LOADER(object);
  AccountPluginLoaderPrivate *priv = PRIVATE(plugin);

  switch (property_id)
  {
    case PROP_PATH:
    {
      g_free(priv->path);
      priv->path = g_value_dup_string(value);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object,
                                        property_id,
                                        pspec);
      break;
    }
  }
}

static void
account_plugin_loader_get_property(GObject *object, guint property_id,
                                   GValue *value, GParamSpec *pspec)
{
  AccountPluginLoader *plugin = ACCOUNT_PLUGIN_LOADER(object);
  AccountPluginLoaderPrivate *priv = PRIVATE(plugin);

  switch (property_id)
  {
    case PROP_PATH:
    {
      g_value_set_string(value, priv->path);
      break;
    }
    default:
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object,
                                        property_id,
                                        pspec);
      break;
    }
  }
}

static gboolean
account_plugin_loader_load(GTypeModule *module)
{
  AccountPluginLoader *plugin = ACCOUNT_PLUGIN_LOADER(module);
  AccountPluginLoaderPrivate *priv = PRIVATE(plugin);

  g_return_val_if_fail(priv->path != NULL, FALSE);

  priv->module = g_module_open(priv->path, G_MODULE_BIND_MASK);

  if (!priv->module)
  {
    g_warning("%s", g_module_error());
    return FALSE;
  }

  if (g_module_symbol(priv->module, "account_plugin_load",
                      (gpointer *)&priv->load) &&
      g_module_symbol(priv->module, "account_plugin_unload",
                      (gpointer *)&priv->unload))
  {
    priv->load(plugin);
    return TRUE;
  }

  g_warning("%s", g_module_error());
  g_module_close(priv->module);

  return FALSE;
}

static void
account_plugin_loader_unload(GTypeModule *module)
{
  AccountPluginLoader *plugin = ACCOUNT_PLUGIN_LOADER(module);
  AccountPluginLoaderPrivate *priv = PRIVATE(plugin);

  priv->unload(plugin);
  g_module_close(priv->module);

  priv->module = NULL;
  priv->load = NULL;
  priv->unload = NULL;
  g_list_free(plugin->gtypes);
  plugin->gtypes = NULL;
}

static void
account_plugin_loader_class_init(AccountPluginLoaderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  GTypeModuleClass *type_module_class = G_TYPE_MODULE_CLASS(klass);

  object_class->finalize = account_plugin_loader_finalize;
  object_class->get_property = account_plugin_loader_get_property;
  object_class->set_property = account_plugin_loader_set_property;
  type_module_class->load = account_plugin_loader_load;
  type_module_class->unload = account_plugin_loader_unload;

  g_object_class_install_property(
    object_class, PROP_PATH,
    g_param_spec_string(
      "path",
      "Plugin path",
      "Path of the plugin",
      NULL,
      G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}

static void
account_plugin_loader_init(AccountPluginLoader *plugin)
{}

/**
 * account_plugin_loader_new:
 * @path: path of the plugin to load.
 *
 * Create an object for loading the plugin located at path.
 *
 * Returns: the #AccountPluginLoader object.
 */
AccountPluginLoader *
account_plugin_loader_new(const gchar *path)
{
  g_return_val_if_fail(path != NULL, NULL);

  return g_object_new(ACCOUNT_TYPE_PLUGIN_LOADER, "path", path, NULL);
}

/**
 * account_plugin_loader_add_type:
 * @plugin_loader: the plugin loader
 * @type: a #GType.
 *
 * Adds @type to the list of types exported by plugin.
 */
void
account_plugin_loader_add_type(AccountPluginLoader *plugin_loader, GType type)
{
  plugin_loader->gtypes = g_list_append(plugin_loader->gtypes,
                                        GSIZE_TO_POINTER(type));
}

/**
 * account_plugin_loader_get_objects:
 * @plugin_loader: plugin loader
 *
 * Lists the #GType<!--> <-->s exported by this plugin.
 *
 * Returns:(transfer full): a #GList of objects for all #GType<!--> <-->s.
 */
GList *
account_plugin_loader_get_objects(AccountPluginLoader *plugin_loader)
{
  GList *types;
  GList *objects = NULL;

  for (types = plugin_loader->gtypes; types; types = types->next)
  {
    objects = g_list_append(objects,
                            g_object_new(GPOINTER_TO_SIZE(types->data), NULL));
  }

  return objects;
}
