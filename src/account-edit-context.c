/*
 * account-edit-context.c
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
 * SECTION:account-edit-context
 * @short_description: context data and methods for account editing.
 *
 * An #AccountEditContext is an object created by the #AccountPlugin for editing
 * or creating an #AccountItem. The base class only provides methods for getting
 * the context data, so account plugins should provide a subclass of it
 * implementing also some interface for performing the actual editing (such as
 * the #AccountWizardContext interface, which is currently the only one
 * available).
 */

#include "config.h"

#include "account-edit-context.h"

struct _AccountEditContextPrivate
{
  AccountItem *account;
  AccountPlugin *plugin;
  gboolean editing;
};

typedef struct _AccountEditContextPrivate AccountEditContextPrivate;

#define PRIVATE(ctx) \
  ((AccountEditContextPrivate *) \
   account_edit_context_get_instance_private((AccountEditContext *)(ctx)))

G_DEFINE_TYPE_WITH_PRIVATE(
  AccountEditContext,
  account_edit_context,
  G_TYPE_OBJECT
)

enum
{
  PROP_ACCOUNT = 1,
  PROP_PLUGIN,
  PROP_EDITING
};

static void
account_edit_context_finalize(GObject *object)
{
  G_OBJECT_CLASS(account_edit_context_parent_class)->finalize(object);
}

static void
account_edit_context_dispose(GObject *object)
{
  AccountEditContextPrivate *priv = PRIVATE(object);

  if (priv->account)
  {
    g_object_unref(priv->account);
    priv->account = NULL;
  }

  if (priv->plugin)
  {
    g_object_unref(priv->plugin);
    priv->plugin = NULL;
  }

  G_OBJECT_CLASS(account_edit_context_parent_class)->dispose(object);
}

static void
account_edit_context_set_property(GObject *object, guint property_id,
                                  const GValue *value, GParamSpec *pspec)
{
  AccountEditContextPrivate *priv;

  g_return_if_fail(ACCOUNT_IS_EDIT_CONTEXT(object));

  priv = PRIVATE(object);

  switch (property_id)
  {
    case PROP_PLUGIN:
    {
      priv->plugin = g_value_dup_object(value);
      break;
    }
    case PROP_EDITING:
    {
      priv->editing = g_value_get_boolean(value);
      break;
    }
    case PROP_ACCOUNT:
    {
      priv->account = g_value_dup_object(value);
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
account_edit_context_get_property(GObject *object, guint property_id,
                                  GValue *value, GParamSpec *pspec)
{
  AccountEditContextPrivate *priv;

  g_return_if_fail(ACCOUNT_IS_EDIT_CONTEXT(object));

  priv = PRIVATE(object);

  switch (property_id)
  {
    case PROP_PLUGIN:
    {
      g_value_set_object(value, priv->plugin);
      break;
    }
    case PROP_EDITING:
    {
      g_value_set_boolean(value, priv->editing);
      break;
    }
    case PROP_ACCOUNT:
    {
      g_value_set_object(value, priv->account);
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
account_edit_context_class_init(AccountEditContextClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = account_edit_context_finalize;
  object_class->dispose = account_edit_context_dispose;
  object_class->set_property = account_edit_context_set_property;
  object_class->get_property = account_edit_context_get_property;

  g_object_class_install_property(
    object_class, PROP_ACCOUNT,
    g_param_spec_object(
      "account",
      "Account",
      "Account item",
      ACCOUNT_TYPE_ITEM,
      G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

  g_object_class_install_property(
    object_class, PROP_PLUGIN,
    g_param_spec_object(
      "plugin",
      "Plugin",
      "Account plugin",
      ACCOUNT_TYPE_PLUGIN,
      G_PARAM_CONSTRUCT_ONLY|G_PARAM_READWRITE));

  g_object_class_install_property(
    object_class, PROP_EDITING,
    g_param_spec_boolean(
      "editing",
      "Editing",
      "Editing an existing account",
      FALSE,
      G_PARAM_CONSTRUCT_ONLY|G_PARAM_READWRITE));
}

static void
account_edit_context_init(AccountEditContext *ctx)
{}

/**
 * account_edit_context_get_account
 * @context: the #AccountEditContext.
 *
 * Gets the account item bound to this context.
 *
 * Returns:(transfer none): the #AccountItem
 */
AccountItem *
account_edit_context_get_account(AccountEditContext *context)
{
  g_return_val_if_fail(ACCOUNT_IS_EDIT_CONTEXT(context), NULL);

  return PRIVATE(context)->account;
}

/**
 * account_edit_context_get_plugin
 * @context: the #AccountEditContext.
 *
 * Gets the account plugin bound to this context.
 *
 * Returns:(transfer none): the #AccountPlugin
 */
AccountPlugin *
account_edit_context_get_plugin(AccountEditContext *context)
{
  g_return_val_if_fail(ACCOUNT_IS_EDIT_CONTEXT(context), NULL);

  return PRIVATE(context)->plugin;
}

/**
 * account_edit_context_get_editing
 * @context: the #AccountEditContext.
 *
 * Returns: %TRUE if the account is an existing account being edited; %FALSE in
 * case it is a newly created one.
 */
gboolean
account_edit_context_get_editing(AccountEditContext *context)
{
  g_return_val_if_fail(ACCOUNT_IS_EDIT_CONTEXT(context), FALSE);

  return PRIVATE(context)->editing;
}
