/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
 * Copyright (C) 2011 Thomas Richard
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

#include "parameter-edit-model.h"

#include "parameter-item.h"

#include <KApplication>
#include <KDebug>
#include <KLocale>
#include <KMessageBox>

#include <QtGui/QValidator>

#include <TelepathyQt4/ConnectionManager>

ParameterEditModel::ParameterEditModel(QObject *parent)
 : QAbstractListModel(parent)
{
    kDebug();

    // TODO: Implement me!
}

ParameterEditModel::~ParameterEditModel()
{
    kDebug();

    // TODO: Implement me!
}

int ParameterEditModel::rowCount(const QModelIndex &index) const
{
    // If the index is the root item, then return the row count.
    if (index == QModelIndex()) {
       return m_items.size();
    }

    // Otherwise, return 0 (as this is a list model, so all items
    // are children of the root item).
    return 0;
}

QVariant ParameterEditModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    // FIXME: This is a basic implementation just so I can see what's going
    // on while developing this code further. Needs expanding.
    QVariant data;

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case ParameterEditModel::ValueRole:
        data = QVariant(m_items.at(index.row())->value());
        if(!data.isValid())
        {
            data = this->data(index, ParameterEditModel::DefaultValueRole);
        }
        break;
    case ParameterEditModel::DefaultValueRole:
        data = QVariant(m_items.at(index.row())->parameter().defaultValue());
        break;
    case ParameterEditModel::NameRole:
        data = QVariant(m_items.at(index.row())->name());
        break;
    case ParameterEditModel::LocalizedNameRole:
        data = QVariant(m_items.at(index.row())->localizedName());
        break;
    case ParameterEditModel::TypeRole:
        data = QVariant(m_items.at(index.row())->type());
        break;
    case ParameterEditModel::SecretRole:
        data = QVariant(m_items.at(index.row())->isSecret());
        break;
    case ParameterEditModel::RequiredRole:
        data = QVariant(m_items.at(index.row())->isRequired());
        break;
    case ParameterEditModel::RequiredForRegistrationRole:
        data = QVariant(m_items.at(index.row())->isRequiredForRegistration());
        break;
    case ParameterEditModel::ValidityRole:
        data = QVariant(m_items.at(index.row())->validity());
        break;
    default:
        break;
    }

    return data;
}

bool ParameterEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        kDebug() << "Invalid item row accessed.";
        return false;
    }

    if (role == ParameterEditModel::ValueRole || role == Qt::EditRole) {

        m_items.at(index.row())->setValue(value);
        Q_EMIT dataChanged(index, index);
        return true;

    } else if (ParameterEditModel::ValidityRole) {

        if (value.toInt() == QValidator::Acceptable) {
            m_items.at(index.row())->setValidity(QValidator::Acceptable);
        } else if (value.toInt() == QValidator::Intermediate) {
            m_items.at(index.row())->setValidity(QValidator::Intermediate);
        } else {
            m_items.at(index.row())->setValidity(QValidator::Invalid);
        }

        Q_EMIT dataChanged(index, index);
        return true;

    } else {
        return false;
    }
}


QModelIndex ParameterEditModel::indexForParameter(const Tp::ProtocolParameter &parameter) const
{
    for(int i=0; i<m_items.size(); ++i) {
        if(m_items.at(i)->parameter() == parameter)
            return createIndex(i,0);
    }
    return QModelIndex();
}

Tp::ProtocolParameter ParameterEditModel::parameter(const QString &parameterName) const
{
    foreach(ParameterItem* item, m_items) {
        if(item->parameter().name() == parameterName) {
            return item->parameter();
        }
    }

    return Tp::ProtocolParameter();
}


void ParameterEditModel::addItem(const Tp::ProtocolParameter &parameter, const QVariant &originalValue)
{
    kDebug();
    // FIXME: Check we are not creating duplicate items.

    // Create a new ParameterItem and add it to the list.
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(new ParameterItem(parameter, originalValue, this));
    endInsertRows();
}

void ParameterEditModel::addItems(const Tp::ProtocolParameterList& parameters, const QVariantMap& parameterValues)
{
    foreach (const Tp::ProtocolParameter &parameter, parameters) {
        addItem(parameter, parameterValues[parameter.name()]);
    }
}


QList<ProtocolParameterValue> ParameterEditModel::parameterValues() const
{
    QList<ProtocolParameterValue> values;

    foreach (ParameterItem *item, m_items) {
        values.append(ProtocolParameterValue(item->parameter(), item->value()));
    }

    return values;
}

Tp::ProtocolParameterList ParameterEditModel::parameters() const
{
    Tp::ProtocolParameterList parameters;

    foreach (ParameterItem *item, m_items) {
        parameters.append(item->parameter());
    }

    return parameters;
}


bool ParameterEditModel::validateParameterValues()
{
    foreach (ParameterItem *item, m_items) {
        if (item->validity() != QValidator::Acceptable) {
           // Display a more helpful error here.
           KMessageBox::error(QApplication::activeWindow (),
                              i18n("\"<b>%1</b>\" is not an acceptable value for <b>%2</b>")
                              .arg(item->value().toString())
                              .arg(item->localizedName()));
           return false;
       }
    }

    return true;
}


#include "parameter-edit-model.moc"

