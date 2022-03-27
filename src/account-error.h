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

#ifndef _ACCOUNT_ERROR_H_
#define _ACCOUNT_ERROR_H_

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
    ACCOUNT_ERROR_UNKNOWN = 0,
    ACCOUNT_ERROR_ITEM_INVALID,
    ACCOUNT_ERROR_CANNOT_NEXT,
    ACCOUNT_ERROR_INVALID_CHARACTER,
    ACCOUNT_ERROR_INVALID_VALUE,
    ACCOUNT_ERROR_ALREADY_EXISTS,
    ACCOUNT_ERROR_PASSWORD_MATCH,
    ACCOUNT_ERROR_REGISTRATION_FAILED,
    ACCOUNT_ERROR_USERNAME_EXISTS,
    ACCOUNT_ERROR_CONNECTION_FAILED,
    ACCOUNT_ERROR_OPERATION_ASYNC,
    ACCOUNT_ERROR_VERIFICATION_FAILED,
    ACCOUNT_ERROR_AUTHENTICATION_FAILED,
    ACCOUNT_ERROR_USER_CANCELLED,
    ACCOUNT_ERROR_NAME_IN_USE
} AccountErrorCode;

#define ACCOUNT_ERROR   g_quark_from_static_string ("account-error-quark")

/* Compatibility measure */
#define ACCOUNT_ERROR_PAGE_ASYNC ACCOUNT_ERROR_OPERATION_ASYNC

G_END_DECLS

#endif /* _ACCOUNT_ERROR_H_ */
