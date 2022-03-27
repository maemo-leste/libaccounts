/*
 * account-wizard-context.c
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
 * SECTION:account-wizard-context
 * @short_description:  interface to provide wizard-like account editing.
 *
 * The #AccountWizardContext interface specifies a small set of methods and
 * properties that an #AccountEditContext subclass can implement in order to
 * provide a wizard-like editing for #AccountItem objects.
 *
 * The most important functions in this interface are
 * account_wizard_context_next() and account_wizard_context_back(): they serve
 * to respectively retrieve the next and the previous page of the wizard, in
 * form of a #GtkWidget. The user interface application is supposed to provide
 * the Cancel, Back, Next and Finish buttons (which therefore must not be
 * present in the page returned by the #AccountWizardContext) and listen to the
 * #AccountWizardContext:can-next, #AccountWizardContext:can-back and
 * #AccountWizardContext:can-finish properties to determine if the buttons
 * should be enabled or disabled.
 *
 * In any case, regardless of the buttons' state, the above mentioned methods
 * should still be called by the UI when the user presses the buttons and it's
 * perfectly valid for these methods to return a %NULL page and set the #GError
 * accordingly (see #AccountErrorCode for a list of possible error codes).
 *
 * The error code %ACCOUNT_ERROR_PAGE_ASYNC has a special meaning: it is used
 * when retrieving the next/previous page is an asyncronous operation (and
 * possibly lengthy). In such cases, the page (or an error code) will be
 * delivered through the #AccountWizardContext::page-async signal.
 *
 * To terminate the editing the UI should call either
 * account_wizard_context_cancel() to abort the account creation/editing or
 * account_wizard_context_finish() to complete the editing process.
 */

#include "config.h"

#include "account-wizard-context.h"

#include "account-marshal.h"

typedef AccountWizardContextIface AccountWizardContextInterface;

G_DEFINE_INTERFACE(
  AccountWizardContext,
  account_wizard_context,
  G_TYPE_OBJECT
)

enum
{
  PAGE_ASYNC,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

#define NO_IMPL(fun) \
    g_warning("AccountWizardContext::" # fun "not implemented for `%s'", \
              g_type_name(G_TYPE_FROM_INSTANCE(context)));

static void
account_wizard_context_default_init(AccountWizardContextIface *iface)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  g_object_interface_install_property(
    iface,
    g_param_spec_boolean(
      "can-next",
      "Can next",
      "Can go next page",
      FALSE,
      G_PARAM_READWRITE));
  g_object_interface_install_property(
    iface,
    g_param_spec_boolean(
      "can-back",
      "Can back",
      "Can go back page",
      FALSE,
      G_PARAM_READWRITE));
  g_object_interface_install_property(
    iface,
    g_param_spec_boolean(
      "can-finish",
      "Can finish",
      "Can finish editing",
      FALSE,
      G_PARAM_READWRITE));

  signals[PAGE_ASYNC] =
    g_signal_new(
      "page-async", G_TYPE_FROM_CLASS(iface), G_SIGNAL_RUN_FIRST, 0, NULL,
      NULL, account_marshal_VOID__OBJECT_POINTER,
      G_TYPE_NONE, 2, GTK_TYPE_WIDGET, G_TYPE_POINTER);

  initialized = TRUE;
}

/**
 * account_wizard_context_get_can_next:
 * @context:the #AccountWizardContext.
 *
 * Whether the "next" button should be enabled.
 *
 * Returns: %TRUE if "next" should be enabled.
 */
gboolean
account_wizard_context_get_can_next(AccountWizardContext *context)
{
  gboolean can_next;

  g_object_get(context, "can-next", &can_next, NULL);

  return can_next;
}

/**
 * account_wizard_context_get_can_back:
 * @context:the #AccountWizardContext.
 *
 * Whether the "back" button should be enabled.
 *
 * Returns: %TRUE if "back" should be enabled.
 */
gboolean
account_wizard_context_get_can_back(AccountWizardContext *context)
{
  gboolean can_back;

  g_object_get(context, "can-back", &can_back, NULL);

  return can_back;
}

/**
 * account_wizard_context_get_can_finish:
 * @context:the #AccountWizardContext.
 *
 * Whether the "finish" button should be enabled.
 *
 * Returns: %TRUE if "finish" should be enabled.
 */
gboolean
account_wizard_context_get_can_finish(AccountWizardContext *context)
{
  gboolean can_finish;

  g_object_get(context, "can-finish", &can_finish, NULL);

  return can_finish;
}

/**
 * account_wizard_context_set_can_next:
 * @context:the #AccountWizardContext.
 * @can_next: %TRUE if "next" should be enabled.
 *
 * Whether the "next" button should be enabled.
 */
void
account_wizard_context_set_can_next(AccountWizardContext *context,
                                    gboolean can_next)
{
  g_object_set(context, "can-next", can_next, NULL);
}

/**
 * account_wizard_context_set_can_back:
 * @context:the #AccountWizardContext.
 * @can_back: %TRUE if "back" should be enabled.
 *
 * Whether the "back" button should be enabled.
 */
void
account_wizard_context_set_can_back(AccountWizardContext *context,
                                    gboolean can_back)
{
  g_object_set(context, "can-back", can_back, NULL);
}

/**
 * account_wizard_context_set_can_finish:
 * @context:the #AccountWizardContext.
 * @can_finish: %TRUE if "finish" should be enabled.
 *
 * Whether the "finish" button should be enabled.
 */
void
account_wizard_context_set_can_finish(AccountWizardContext *context,
                                      gboolean can_finish)
{
  g_object_set(context, "can-finish", can_finish, NULL);
}

/**
 * account_wizard_context_next:
 * @context: the #AccountWizardContext.
 * @error: a pointer to a #GError.
 *
 * Proceeds to the next page of the wizard.
 *
 * Returns:(transfer none): the #GtkWidget to be displayed on the next page.
 */
GtkWidget *
account_wizard_context_next(AccountWizardContext *context, GError **error)
{
  AccountWizardContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_WIZARD_CONTEXT(context), NULL);

  iface = ACCOUNT_WIZARD_CONTEXT_GET_IFACE(context);

  if (iface->next)
    return iface->next(context, error);

  NO_IMPL(next);

  return NULL;
}

/**
 * account_wizard_context_back:
 * @context: the #AccountWizardContext.
 * @error: a pointer to a #GError.
 *
 * Goes back to the previous page of the wizard.
 *
 * Returns:(transfer none): the #GtkWidget to be displayed on the previous page.
 */
GtkWidget *
account_wizard_context_back(AccountWizardContext *context, GError **error)
{
  AccountWizardContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_WIZARD_CONTEXT(context), NULL);

  iface = ACCOUNT_WIZARD_CONTEXT_GET_IFACE(context);

  if (iface->back)
    return iface->back(context, error);

  NO_IMPL(back);

  return NULL;
}

/**
 * account_wizard_context_get_page_title:
 * @context: the #AccountWizardContext.
 *
 * Gets the title of the current page.
 *
 * Returns:(transfer none): a string representing the title.
 */
const gchar *
account_wizard_context_get_page_title(AccountWizardContext *context)
{
  AccountWizardContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_WIZARD_CONTEXT(context), NULL);

  iface = ACCOUNT_WIZARD_CONTEXT_GET_IFACE(context);

  if (iface->get_page_title)
    return iface->get_page_title(context);

  NO_IMPL(back);

  return NULL;
}

/**
 * account_wizard_context_finish:
 * @context: the #AccountWizardContext.
 * @error: a pointer to a #GError.
 *
 * Completes the account creation. If this call succeeds, the context can be
 * destroyed and the newly created #AccountItem will be added into the
 * #AccountsList.
 *
 * Returns: %TRUE if account was created successfully, %FALSE otherwise.
 */
gboolean
account_wizard_context_finish(AccountWizardContext *context, GError **error)
{
  AccountWizardContextIface *iface;

  g_return_val_if_fail(ACCOUNT_IS_WIZARD_CONTEXT(context), FALSE);

  iface = ACCOUNT_WIZARD_CONTEXT_GET_IFACE(context);

  if (iface->finish)
    return iface->finish(context, error);

  NO_IMPL(finish);

  return FALSE;
}

/**
 * account_wizard_context_cancel:
 * @context: the #AccountWizardContext.
 *
 * Cancel the account creation. The context can then be destroyed.
 */
void
account_wizard_context_cancel(AccountWizardContext *context)
{
  AccountWizardContextIface *iface;

  g_return_if_fail(ACCOUNT_IS_WIZARD_CONTEXT(context));

  iface = ACCOUNT_WIZARD_CONTEXT_GET_IFACE(context);

  if (iface->cancel)
    iface->cancel(context);
  else
    NO_IMPL(finish);
}
