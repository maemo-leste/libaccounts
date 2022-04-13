/*
 * account-service.c
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

#include "account-service.h"

/**
 * SECTION:account-service
 * @short_description: A service accessible by accounts.
 *
 * An #AccountService represents a service which accounts can access. Services
 * are created by #AccountPlugin objects and are means by which the same plugin
 * could provide support for different types of accounts.
 */

G_DEFINE_TYPE(
  AccountService,
  account_service,
  G_TYPE_OBJECT
)

enum
{
  PROP_NAME = 1,
  PROP_DISPLAY_NAME,
  PROP_SUPPORTS_AVATAR,
  PROP_ICON,
  PROP_PLUGIN,
  PROP_SERVICE_NAME
};

static void
account_service_dispose(GObject *object)
{
  AccountService *service = ACCOUNT_SERVICE(object);

  if (service->plugin)
  {
    g_object_remove_weak_pointer(G_OBJECT(service->plugin),
                                 (gpointer *)&(service->plugin));
    service->plugin = NULL;
  }

  if (service->icon)
  {
    g_object_unref(service->icon);
    service->icon = NULL;
  }

  G_OBJECT_CLASS(account_service_parent_class)->dispose(object);
}

static void
account_service_finalize(GObject *object)
{
  AccountService *service = ACCOUNT_SERVICE(object);

  g_free(service->name);
  g_free(service->display_name);
  g_free(service->service_name);

  G_OBJECT_CLASS(account_service_parent_class)->finalize(object);
}

static void
account_service_set_property(GObject *object, guint property_id,
                             const GValue *value, GParamSpec *pspec)
{
  AccountService *service;

  g_return_if_fail(ACCOUNT_IS_SERVICE(object));

  service = ACCOUNT_SERVICE(object);

  switch (property_id)
  {
    case PROP_NAME:
    {
      g_free(service->name);
      service->name = g_value_dup_string(value);
      break;
    }
    case PROP_DISPLAY_NAME:
    {
      g_free(service->display_name);
      service->display_name = g_value_dup_string(value);
      break;
    }
    case PROP_SUPPORTS_AVATAR:
    {
      service->supports_avatar = g_value_get_boolean(value);
      break;
    }
    case PROP_ICON:
    {
      service->icon = service->icon;

      if (service->icon)
        g_object_unref(service->icon);

      service->icon = g_value_dup_object(value);
      break;
    }
    case PROP_PLUGIN:
    {
      g_return_if_fail(service->plugin == NULL);

      service->plugin = g_value_get_object(value);
      g_object_add_weak_pointer(G_OBJECT(service->plugin),
                                (gpointer *)&(service->plugin));

      break;
    }
    case PROP_SERVICE_NAME:
    {
      g_free(service->service_name);
      service->service_name = g_value_dup_string(value);
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
account_service_get_property(GObject *object, guint property_id, GValue *value,
                             GParamSpec *pspec)
{
  AccountService *service;

  g_return_if_fail(ACCOUNT_IS_SERVICE(object));

  service = ACCOUNT_SERVICE(object);

  switch (property_id)
  {
    case PROP_NAME:
    {
      g_value_set_string(value, service->name);
      break;
    }
    case PROP_DISPLAY_NAME:
    {
      g_value_set_string(value, service->display_name);
      break;
    }
    case PROP_SUPPORTS_AVATAR:
    {
      g_value_set_boolean(value, service->supports_avatar);
      break;
    }
    case PROP_ICON:
    {
      g_value_set_object(value, service->icon);
      break;
    }
    case PROP_PLUGIN:
    {
      g_value_set_object(value, service->plugin);
      break;
    }
    case PROP_SERVICE_NAME:
    {
      g_value_set_string(value, service->service_name);
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
account_service_class_init(AccountServiceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = account_service_dispose;
  object_class->finalize = account_service_finalize;
  object_class->set_property = account_service_set_property;
  object_class->get_property = account_service_get_property;

  g_object_class_install_property(
    object_class, PROP_NAME,
    g_param_spec_string(
      "name",
      "Name",
      "Account name",
      NULL,
      G_PARAM_CONSTRUCT | G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_DISPLAY_NAME,
    g_param_spec_string(
      "display-name",
      "Display name",
      "Display name",
      NULL,
      G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SUPPORTS_AVATAR,
    g_param_spec_boolean(
      "supports-avatar",
      "Supports avatar",
      "Supports avatar",
      FALSE,
      G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_ICON,
    g_param_spec_object(
      "icon",
      "Icon",
      "Icon",
      GDK_TYPE_PIXBUF,
      G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_PLUGIN,
    g_param_spec_object(
      "plugin",
      "Plugin",
      "Plugin",
      ACCOUNT_TYPE_PLUGIN,
      G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SERVICE_NAME,
    g_param_spec_string(
      "service-name",
      "Service name",
      "Service name",
      NULL,
      G_PARAM_WRITABLE | G_PARAM_READABLE));
}

static void
account_service_init(AccountService *service)
{}

/**
 * account_service_get_plugin:
 * @service: the #AccountService.
 *
 * Gets the plugin which manages this account service.
 *
 * Returns:(transfer none): the #AccountPlugin of the @service
 */
AccountPlugin *
account_service_get_plugin(AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), NULL);

  return service->plugin;
}

/**
 * account_service_begin_new:
 * @service: The #AccountService.
 *
 * Start the creation of a new account for accessing this service.
 *
 * Returns:(transfer full): an #AccountEditContext for editing the account.
 */
AccountEditContext *
account_service_begin_new(AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), NULL);

  return account_plugin_begin_new(service->plugin, service);
}

/**
 * account_service_get_name:
 * @service: the #AccountService.
 *
 * Gets the name of this service.
 *
 * Returns:(transfer none): a string representing the service's name.
 */
const gchar *
account_service_get_name(AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), NULL);

  return service->name;
}

/**
 * account_service_get_display_name:
 * @service: the #AccountService.
 *
 * Gets the name of this service.
 *
 * Returns:(transfer none): a string representing the service's display name.
 */
const gchar *
account_service_get_display_name(AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), NULL);

  return service->display_name;
}

/**
 * account_service_get_priority:
 * @service: the #AccountService.
 *
 * Gets the priority of this service.
 *
 * Returns: service priority
 */
gint
account_service_get_priority (AccountService *service)
{
  g_return_val_if_fail(ACCOUNT_IS_SERVICE(service), 0);

  return service->priority;
}
