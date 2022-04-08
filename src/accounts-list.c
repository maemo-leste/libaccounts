/*
 * accounts-list.c
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
 * SECTION:accounts-list
 * @short_description: list of account items.
 *
 * The #AccountsList interface consists of a few methods to manipulate a list of
 * #AccountItem objects. It is typically implemented by the user interface,
 * which therefore will adjust its view accordingly. Account plugins report to
 * it whenever an #AccountItem is created or deleted, and on the other end
 * listen to the #AccountsList::remove-item signal to know when an account has to
 * be deleted.
 */

#include "config.h"

#include "accounts-list.h"

typedef AccountsListIface AccountsListInterface;

G_DEFINE_INTERFACE(
  AccountsList,
  accounts_list,
  G_TYPE_OBJECT
)

enum
{
  ADD_ITEM,
  REMOVE_ITEM,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = {0};

static void
accounts_list_default_init(AccountsListIface *iface)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  signals[ADD_ITEM] = g_signal_new(
      "add-item", G_TYPE_FROM_CLASS(iface),
      G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST,
      G_STRUCT_OFFSET(AccountsListIface, add), NULL, NULL,
      g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE, 1, ACCOUNT_TYPE_ITEM);
  signals[REMOVE_ITEM] = g_signal_new(
      "remove-item", G_TYPE_FROM_CLASS(iface),
      G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST,
      G_STRUCT_OFFSET(AccountsListIface, remove), NULL, NULL,
      g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE, 1, ACCOUNT_TYPE_ITEM);
  initialized = TRUE;
}

/**
 * accounts_list_add:
 * @accounts_list: the #AccountsList.
 * @account_item: an #AccountItem.
 *
 * Adds @account_item to @accounts_list.
 */
void
accounts_list_add(AccountsList *accounts_list, AccountItem *account_item)
{
  g_return_if_fail(accounts_list != NULL);
  g_return_if_fail(ACCOUNTS_IS_LIST(accounts_list));

  g_signal_emit(accounts_list, signals[ADD_ITEM], 0, account_item);
}

/**
 * accounts_list_remove:
 * @accounts_list: the #AccountsList.
 * @account_item: an #AccountItem.
 *
 * Removes account_item from accounts_list. #AccountsList::remove-item is being
 * emitted on removal.
 */
void
accounts_list_remove(AccountsList *accounts_list, AccountItem *account_item)
{
  g_return_if_fail(accounts_list != NULL);
  g_return_if_fail(ACCOUNTS_IS_LIST(accounts_list));

  g_signal_emit(accounts_list, signals[REMOVE_ITEM], 0, account_item);
}

/**
 * accounts_list_get_all:
 * @accounts_list: the #AccountsList.
 *
 * Lists all accounts registered in @accounts_list.
 *
 * Returns:(transfer full): a #GList of #AccountItem objects.
 */
GList *
accounts_list_get_all(AccountsList *accounts_list)
{
  g_return_val_if_fail(accounts_list != NULL, NULL);
  g_return_val_if_fail(ACCOUNTS_IS_LIST(accounts_list), NULL);

  return ACCOUNTS_LIST_GET_IFACE(accounts_list)->get_all(accounts_list);
}
