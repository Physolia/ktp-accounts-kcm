/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
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

#ifndef TELEPATHY_ACCOUNTS_KCM_ADD_ACCOUNT_ASSISTANT_H
#define TELEPATHY_ACCOUNTS_KCM_ADD_ACCOUNT_ASSISTANT_H

#include <KAssistantDialog>

#include <TelepathyQt4/AccountManager>

namespace Tp {
    class PendingOperation;
}

class AddAccountAssistant : public KAssistantDialog
{
    Q_OBJECT

public:
    explicit AddAccountAssistant(Tp::AccountManagerPtr accountManager, QWidget *parent = 0);
    ~AddAccountAssistant();

protected Q_SLOTS:
    virtual void next();
    virtual void accept();
    virtual void reject();

Q_SIGNALS:
    void cancelled();
	void protocolSelected(QString, QString);

public Q_SLOTS:
	void onTitleForCustomPages(QString mandatoryPage, QList<QString> optionalPage);

private Q_SLOTS:
    void onAccountCreated(Tp::PendingOperation *op);
    void onSetEnabledFinished(Tp::PendingOperation *op);
    void onProtocolSelected(bool value);
	void onProtocolDoubleClicked();

private:
    class Private;
    Private * const d;
};


#endif  // Header guard

