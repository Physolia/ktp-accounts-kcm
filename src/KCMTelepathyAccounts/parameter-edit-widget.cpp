/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include "parameter-edit-widget.h"

#include "parameter-edit-delegate.h"
#include "parameter-edit-model.h"

#include "ui_parameter-edit-widget.h"

#include <KDebug>

class ParameterEditWidget::Private
{
public:
    Private()
     : ui(0), delegate(0)
    {
        kDebug();
    }

    Ui::ParameterEditWidget *ui;
    ParameterEditDelegate *delegate;
};

ParameterEditWidget::ParameterEditWidget(ParameterEditModel *model,
                                         QWidget *parent)
 : AbstractAccountParametersWidget(model, parent),
   d(new Private)
{
    kDebug();

    // Set up the UI.
    d->ui = new Ui::ParameterEditWidget;
    d->ui->setupUi(this);

    d->ui->parameterListView->setModel(this->model());
    d->delegate = new ParameterEditDelegate(d->ui->parameterListView, this);
    d->ui->parameterListView->setItemDelegate(d->delegate);

    connect(d->delegate,
            SIGNAL(dataChanged(QModelIndex, QVariant, int)),
            SLOT(onDelegateDataChanged(QModelIndex, QVariant, int)));

    /*// Add the parameters to the model.
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        d->model->addItem(parameter, values.value(parameter.name(), parameter.defaultValue()));
    }*/
}

ParameterEditWidget::~ParameterEditWidget()
{
    kDebug();

    delete d->ui;
    delete d;
}

QList<ProtocolParameterValue> ParameterEditWidget::parameterValues() const
{
    return model()->parameterValues();
}


void ParameterEditWidget::onDelegateDataChanged(const QModelIndex &index, const QVariant &value, int role)
{
    model()->setData(index, value, role);
}

bool ParameterEditWidget::validateParameterValues()
{
    return model()->validateParameterValues();
}

#include "parameter-edit-widget.moc"

