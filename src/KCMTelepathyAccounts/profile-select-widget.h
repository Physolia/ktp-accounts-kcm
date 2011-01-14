/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Thomas Richard <thomas.richard@proan.be>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_PROFILE_SELECT_WIDGET_H
#define TELEPATHY_ACCOUNTS_KCM_PROFILE_SELECT_WIDGET_H

#include <kdemacros.h>

#include <QtGui/QWidget>

namespace Tp {
    class PendingOperation;
}

class ProfileItem;
class QItemSelection;

class QModelIndex;

class KDE_EXPORT ProfileSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileSelectWidget(QWidget *parent = 0);
    ~ProfileSelectWidget();

    ProfileItem *selectedProfile();

private Q_SLOTS:
    void onProfileManagerReady(Tp::PendingOperation *op);
    void onSelectionChanged(const QItemSelection &selected);

Q_SIGNALS:
    void profileGotSelected(bool selected);
    void profileDoubleClicked();

private:
    class Private;
    Private * const d;
};


#endif  // Header guard

