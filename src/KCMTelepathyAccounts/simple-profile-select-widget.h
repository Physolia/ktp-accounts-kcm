/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TELEPATHY_ACCOUNTS_KCM_SIMPLE_PROFILE_SELECT_WIDGET_H
#define TELEPATHY_ACCOUNTS_KCM_SIMPLE_PROFILE_SELECT_WIDGET_H

#include "kcm_telepathy_accounts_export.h"

#include <QtGui/QWidget>

namespace Tp {
    class PendingOperation;
}

class ProfileItem;
class QItemSelection;

class QModelIndex;

class KCM_TELEPATHY_ACCOUNTS_EXPORT SimpleProfileSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleProfileSelectWidget(QWidget *parent = 0);
    ~SimpleProfileSelectWidget();

    ProfileItem *selectedProfile();

private Q_SLOTS:
    void onProfileManagerReady(Tp::PendingOperation *op);
    void onProfileClicked(QString profileName);

Q_SIGNALS:
    void profileChosen();
    void othersChosen();

private:
    class Private;
    Private * const d;
};


#endif  // Header guard

