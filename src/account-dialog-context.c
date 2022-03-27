/*
 * account-dialog-context.c
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

#include "account-dialog-context.h"

typedef AccountDialogContextIface AccountDialogContextInterface;

G_DEFINE_INTERFACE(
  AccountDialogContext,
  account_dialog_context,
  G_TYPE_OBJECT
)

enum
{
  OPERATION_ASYNC,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

#define WARN_NOT_IMPLEMENTED(fun) \
  g_warning("AccountDialogContext::" # fun "not implemented for `%s'", \
            g_type_name(G_TYPE_FROM_INSTANCE(context))); \

static void
account_dialog_context_default_init(AccountDialogContextIface *iface)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  signals[OPERATION_ASYNC] =
    g_signal_new(
      "operation-async", G_TYPE_FROM_CLASS(iface), G_SIGNAL_RUN_FIRST, 0,
      NULL, NULL, g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1,
      G_TYPE_POINTER);
  initialized = TRUE;
}

GtkWidget *
account_dialog_context_start(AccountDialogContext *context, GError **error)
{
  AccountDialogContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_DIALOG_CONTEXT(context), NULL);

  iface = ACCOUNT_DIALOG_CONTEXT_GET_IFACE(context);

  if (iface->start)
    return iface->start(context, error);

  WARN_NOT_IMPLEMENTED(start);

  return NULL;
}

void
account_dialog_context_cancel(AccountDialogContext *context)
{
  AccountDialogContextIface *iface;

  g_return_if_fail(ACCOUNT_IS_DIALOG_CONTEXT(context));

  iface = ACCOUNT_DIALOG_CONTEXT_GET_IFACE(context);

  if (iface->cancel)
    iface->cancel(context);
  else
    WARN_NOT_IMPLEMENTED(cancel);
}

gboolean
account_dialog_context_finish(AccountDialogContext *context, GError **error)
{
  AccountDialogContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_DIALOG_CONTEXT(context), FALSE);

  iface = ACCOUNT_DIALOG_CONTEXT_GET_IFACE(context);

  if (iface->finish)
    return iface->finish(context, error);

  WARN_NOT_IMPLEMENTED(finish);

  return FALSE;
}

const gchar *
account_dialog_context_get_page_title(AccountDialogContext *context)
{
  AccountDialogContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_DIALOG_CONTEXT(context), NULL);

  iface = ACCOUNT_DIALOG_CONTEXT_GET_IFACE(context);

  if (iface->get_page_title)
    return iface->get_page_title(context);

  WARN_NOT_IMPLEMENTED(get_page_title);

  return NULL;
}
