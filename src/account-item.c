/*
 * account-item.c
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
 * SECTION:account-item
 * @short_description: a representation of an account.
 *
 * An #AccountItem is an object which represents an account. It provides a
 * method for enabling/disabling the account and methods for retrieving the
 * #AccountService and #AccountPlugin it belongs to; the account data are
 * exposed as properties.
 *
 * Account plugins must subclass of #AccountItem and implement the
 * account_item_set_enabled() method.
 */

#include "config.h"

#include "account-item.h"

G_DEFINE_TYPE(
  AccountItem,
  account_item,
  G_TYPE_OBJECT
)

enum
{
  PROP_NAME = 1,
  PROP_DISPLAY_NAME,
  PROP_AVATAR,
  PROP_SUPPORS_AVATAR,
  PROP_SEVICE_NAME,
  PROP_SEVICE_ICON,
  PROP_SEVICE,
  PROP_ENABLED,
  PROP_DRAFT,
  PROP_CONNECTED,
};

enum
{
  VERIFIED,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

#define NOT_IMPLEMENTED(fun, parameters, code) \
  _account_item_ ## fun parameters \
  { \
    g_warning("AccountItem::" # fun "not implemented for `%s'", \
              g_type_name(G_TYPE_FROM_INSTANCE(item))); \
    code \
  }

/* *INDENT-OFF* */
static gboolean
NOT_IMPLEMENTED(set_enabled,
                (AccountItem *item, gboolean enabled, GError **error),
                return FALSE;)
/* *INDENT-ON* */

static void
account_item_dispose(GObject *object)
{
  AccountItem *item = ACCOUNT_ITEM(object);

  if (item->avatar)
  {
    g_object_unref(item->avatar);
    item->avatar = NULL;
  }

  if (item->service_icon)
  {
    g_object_unref(item->service_icon);
    item->service_icon = NULL;
  }

  if (item->service)
  {
    g_object_unref(item->service);
    item->service = NULL;
  }

  G_OBJECT_CLASS(account_item_parent_class)->dispose(object);
}

static void
account_item_finalize(GObject *object)
{
  AccountItem *item = ACCOUNT_ITEM(object);

  g_free(item->name);
  g_free(item->display_name);
  g_free(item->service_name);

  G_OBJECT_CLASS(account_item_parent_class)->finalize(object);
}

static void
account_item_set_property(GObject *object, guint property_id,
                          const GValue *value, GParamSpec *pspec)
{
  AccountItem *item;

  g_return_if_fail(ACCOUNT_IS_ITEM(object));

  item = ACCOUNT_ITEM(object);

  switch (property_id)
  {
    case PROP_NAME:
    {
      g_free(item->name);
      item->name = g_value_dup_string(value);
      break;
    }
    case PROP_DISPLAY_NAME:
    {
      g_free(item->display_name);
      item->display_name = g_value_dup_string(value);
      break;
    }
    case PROP_AVATAR:
    {
      if (item->avatar)
        g_object_unref(item->avatar);

      item->avatar = g_value_dup_object(value);
      break;
    }
    case PROP_SUPPORS_AVATAR:
    {
      item->supports_avatar = g_value_get_boolean(value);
      break;
    }
    case PROP_SEVICE:
    {
      gboolean supports_avatar;
      g_return_if_fail(item->service == NULL);

      item->service = g_value_dup_object(value);

      if (item->service)
      {
        g_return_if_fail(item->service_name == NULL);
        g_return_if_fail(item->service_icon == NULL);

        g_object_get(item->service,
                     "display-name", &item->service_name,
                     "icon", &item->service_icon,
                     "supports-avatar", &supports_avatar,
                     NULL);
        item->supports_avatar = supports_avatar;
      }

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
account_item_get_property(GObject *object, guint property_id, GValue *value,
                          GParamSpec *pspec)
{
  AccountItem *item;

  g_return_if_fail(ACCOUNT_IS_ITEM(object));

  item = ACCOUNT_ITEM(object);

  switch (property_id)
  {
    case PROP_NAME:
    {
      g_value_set_string(value, item->name);
      break;
    }
    case PROP_DISPLAY_NAME:
    {
      g_value_set_string(value, item->display_name);
      break;
    }
    case PROP_AVATAR:
    {
      g_value_set_object(value, item->avatar);
      break;
    }
    case PROP_SUPPORS_AVATAR:
    {
      g_value_set_boolean(value, item->supports_avatar);
      break;
    }
    case PROP_SEVICE_NAME:
    {
      g_value_set_string(value, item->service_name);
      break;
    }
    case PROP_SEVICE_ICON:
    {
      g_value_set_object(value, item->service_icon);
      break;
    }
    case PROP_SEVICE:
    {
      g_value_set_object(value, item->service);
      break;
    }
    case PROP_ENABLED:
    {
      g_value_set_boolean(value, item->enabled);
      break;
    }
    case PROP_DRAFT:
    {
      g_value_set_boolean(value, item->draft);
      break;
    }
    case PROP_CONNECTED:
    {
      g_value_set_boolean(value, item->connected);
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
account_item_class_init(AccountItemClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = account_item_dispose;
  object_class->finalize = account_item_finalize;
  object_class->set_property = account_item_set_property;
  object_class->get_property = account_item_get_property;

  klass->set_enabled = _account_item_set_enabled;

  g_object_class_install_property(
    object_class, PROP_NAME,
    g_param_spec_string("name",
                        "Name",
                        "Account name", NULL,
                        G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_DISPLAY_NAME,
    g_param_spec_string("display-name",
                        "Display name",
                        "Display name",
                        NULL,
                        G_PARAM_WRITABLE | G_PARAM_READABLE));

  g_object_class_install_property(
    object_class, PROP_AVATAR,
    g_param_spec_object("avatar",
                        "Avatar",
                        "Account avatar",
                        GDK_TYPE_PIXBUF,
                        G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SUPPORS_AVATAR,
    g_param_spec_boolean("supports-avatar",
                         "Supports avatar",
                         "Supports avatar",
                         FALSE,
                         G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SEVICE_NAME,
    g_param_spec_string("service-name",
                        "Service name",
                        "Service name",
                        0,
                        G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SEVICE_ICON,
    g_param_spec_object("service-icon",
                        "Service icon",
                        "Service icon",
                        GDK_TYPE_PIXBUF,
                        G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_SEVICE,
    g_param_spec_object(
      "service",
      "Service",
      "Service",
      ACCOUNT_TYPE_SERVICE,
      G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_ENABLED,
    g_param_spec_boolean("enabled",
                         "Enabled",
                         "Account enabled",
                         FALSE,
                         G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_CONNECTED,
    g_param_spec_boolean("connected",
                         "Connected",
                         "Account connected",
                         FALSE,
                         G_PARAM_READABLE));
  g_object_class_install_property(
    object_class, PROP_DRAFT,
    g_param_spec_boolean("draft",
                         "Draft",
                         "Account in draft status",
                         FALSE,
                         G_PARAM_READABLE));
  signals[VERIFIED] = g_signal_new(
      "verified", G_TYPE_FROM_CLASS(klass),
      G_SIGNAL_ACTION | G_SIGNAL_RUN_LAST, 0, NULL, NULL,
      g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static void
account_item_init(AccountItem *item)
{}

/**
 * account_item_set_enabled:
 * @account: the #AccountItem.
 * @enabled: whether to enable or disable the account.
 * @error: a GError for error reporting, or %NULL.
 *
 * Enables or disables @account.
 *
 * This is a virtual method that must be implemented by account plugins by
 * subclassing #AccountItem. The method's task is to enable/disable the account;
 * to do so, set the #AccountItem:enabled member to the desired value and call
 * g_object_notify() on the #AccountItem:enabled property.
 *
 * Returns: %TRUE if success, %FALSE on error.
 *
 * <note>
 * In versions up to 3.73, the #AccountItem:enabled property was set
 * automatically, consistently with the method's return value.
 * </note>
 */
gboolean
account_item_set_enabled(AccountItem *account, gboolean enabled, GError **error)
{
  g_return_val_if_fail(ACCOUNT_IS_ITEM(account), FALSE);

  return ACCOUNT_ITEM_GET_CLASS(account)->set_enabled(account, enabled, error);
}

/**
 * account_item_get_plugin:
 * @account: the #AccountItem.
 *
 * Gets the plugin which manages this account item.
 *
 * Returns:(transfer none): the #AccountPlugin of this @account
 *
 */
AccountPlugin *
account_item_get_plugin(AccountItem *account)
{
  g_return_val_if_fail(ACCOUNT_IS_ITEM(account), NULL);

  if (account->service)
    return account_service_get_plugin(account->service);

  return NULL;
}

/**
 * account_item_get_service:
 * @account: the #AccountItem.
 *
 * Gets the service which this account item provides.
 *
 * Returns:(transfer none): the #AccountService of this account
 */
AccountService *
account_item_get_service(AccountItem *account)
{
  g_return_val_if_fail(ACCOUNT_IS_ITEM(account), NULL);

  return account->service;
}

gboolean
account_item_is_connected(AccountItem *account)
{
  g_return_val_if_fail(ACCOUNT_IS_ITEM(account), FALSE);

  return account->connected;
}
