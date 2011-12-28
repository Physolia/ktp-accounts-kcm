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

#include "simple-profile-select-widget.h"

#include "profile-item.h"
#include "profile-list-model.h"

#include "ui_simple-profile-select-widget.h"

#include <KDebug>

#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ProfileManager>
#include <TelepathyQt/Feature>

#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QCommandLinkButton>
#include <QSignalMapper>

class SimpleProfileSelectWidget::Private
{
public:
    Private()
     : profileManager(0),
       ui(0),
       signalMapper(0),
       profileItem(0)
    {
        kDebug();
    }

    Tp::ProfileManagerPtr profileManager;
    Ui::SimpleProfileSelectWidget *ui;
    QSignalMapper *signalMapper;
    ProfileItem *profileItem;
    QString profileName;
};

SimpleProfileSelectWidget::SimpleProfileSelectWidget(QWidget *parent)
 : QWidget(parent),
   d(new Private)
{
    kDebug();

    // Set up the models
    d->signalMapper = new QSignalMapper(this);

    // Set up the widget
    d->ui = new Ui::SimpleProfileSelectWidget;
    d->ui->setupUi(this);

    // Create the buttons for the Major Profiles
    QCommandLinkButton *buttonJabber = new QCommandLinkButton("Jabber / XMPP");
    buttonJabber->setIcon(KIcon("im-jabber"));
    buttonJabber->setIconSize(QSize(32,32));

    QCommandLinkButton *buttonGTalk = new QCommandLinkButton("Google Talk");
    buttonGTalk->setIcon(KIcon("im-google-talk"));
    buttonGTalk->setIconSize(QSize(32,32));

    QCommandLinkButton *buttonFacebook = new QCommandLinkButton("Facebook Chat");
    buttonFacebook->setIcon(KIcon("im-facebook"));
    buttonFacebook->setIconSize(QSize(32,32));

    QCommandLinkButton *buttonIcq = new QCommandLinkButton("ICQ");
    buttonIcq->setIcon(KIcon("im-icq"));
    buttonIcq->setIconSize(QSize(32,32));

    QCommandLinkButton *buttonMsn = new QCommandLinkButton("Windows Live Messenger");
    buttonMsn->setIcon(KIcon("im-msn"));
    buttonMsn->setIconSize(QSize(32,32));

    QCommandLinkButton *buttonOthers = new QCommandLinkButton("Others", "AOL, Gadu-Gadu, IRC, Yahoo and more...");
    buttonOthers->setIcon(KIcon("go-next"));
    buttonOthers->setIconSize(QSize(32,32));

    // Add them to the SignalMapper
    d->signalMapper->setMapping(buttonJabber,   "jabber");
    d->signalMapper->setMapping(buttonGTalk,    "google-talk");
    d->signalMapper->setMapping(buttonFacebook, "facebook");
    d->signalMapper->setMapping(buttonIcq,      "haze-icq");
    d->signalMapper->setMapping(buttonMsn,      "haze-msn");

    // Connect them to the SignalMapper
    connect(buttonJabber,   SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(buttonGTalk,    SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(buttonFacebook, SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(buttonIcq,      SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(buttonMsn,      SIGNAL(clicked()), d->signalMapper, SLOT(map()));
    connect(buttonOthers,   SIGNAL(clicked()), this,            SIGNAL(othersChosen()));

    connect(d->signalMapper,SIGNAL(mapped(const QString &)),
            this,           SLOT(onProfileClicked(const QString &)));

    // Add them to the Layout
    d->ui->verticalLayout->addWidget(buttonJabber);
    d->ui->verticalLayout->addWidget(buttonGTalk);
    d->ui->verticalLayout->addWidget(buttonFacebook);
    d->ui->verticalLayout->addWidget(buttonIcq);
    d->ui->verticalLayout->addWidget(buttonMsn);
    d->ui->verticalLayout->addWidget(buttonOthers);
    d->ui->verticalLayout->addStretch();

    // Disable everything until Tp::ProfileManager is ready
    d->ui->verticalLayout->setEnabled(false);

    d->profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());

    // FIXME: Until all distros ship correct profile files, we should fake them
    connect(d->profileManager->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

SimpleProfileSelectWidget::~SimpleProfileSelectWidget()
{
    kDebug();

    delete d->ui;
    delete d;
}

void SimpleProfileSelectWidget::onProfileManagerReady(Tp::PendingOperation *op)
{
    kDebug();

    // Check the pending operation completed successfully.
    if (op->isError()) {
        kDebug() << "becomeReady() failed:" << op->errorName() << op->errorMessage();
        return;
    }
    else {
        // Enable the buttons
        d->ui->verticalLayout->setEnabled(true);
    }
}

// Return the selected ProfileItem or 0 if nothing is selected.
ProfileItem *SimpleProfileSelectWidget::selectedProfile()
{
    kDebug();

    if (d->profileItem != NULL) {
        delete d->profileItem;
    }

    Tp::ProfilePtr profilePtr = d->profileManager->profileForService(d->profileName);

    if (profilePtr.isNull()) {
        kDebug() << "Profile is missing, we need to install some packages here.";
        return 0;
    }
    else {
        d->profileItem = new ProfileItem(profilePtr, this);
        return d->profileItem;
    }

}

// FIXME: before we proceed here, we should check if there's everything installed we need.
void SimpleProfileSelectWidget::onProfileClicked(QString profileName)
{
    kDebug();

    d->profileName = profileName;

    kDebug() << "Selected profile: " << d->profileName;

    Q_EMIT profileChosen();
}

#include "simple-profile-select-widget.moc"

