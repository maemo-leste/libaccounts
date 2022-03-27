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

#ifndef _ACCOUNTS_LIST_H_
#define _ACCOUNTS_LIST_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define ACCOUNTS_TYPE_LIST             (accounts_list_get_type ())
#define ACCOUNTS_LIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ACCOUNTS_TYPE_LIST, AccountsList))
#define ACCOUNTS_IS_LIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ACCOUNTS_TYPE_LIST))
#define ACCOUNTS_LIST_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ACCOUNTS_TYPE_LIST, AccountsListIface))

typedef struct _AccountsListIface AccountsListIface;
typedef struct _AccountsList AccountsList;

#include "account-item.h"

struct _AccountsListIface
{
    GTypeInterface g_iface;

    /* methods */
    void    (* add)     (AccountsList *accounts_list,
                         AccountItem  *item);
    void    (* remove)  (AccountsList *accounts_list,
                         AccountItem  *item);
    GList*  (* get_all) (AccountsList *accounts_list);
};

GType  accounts_list_get_type (void) G_GNUC_CONST;

void   accounts_list_add (AccountsList *accounts_list, AccountItem *account_item);
void   accounts_list_remove (AccountsList *accounts_list, AccountItem *account_item);

GList *accounts_list_get_all (AccountsList *accounts_list);

G_END_DECLS

#endif /* _ACCOUNTS_LIST_H_ */
