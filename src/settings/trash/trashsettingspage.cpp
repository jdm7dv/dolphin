/***************************************************************************
 *   Copyright (C) 2009 by Shaun Reich shaun.reich@kdemail.net             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "trashsettingspage.h"

#include <KCModuleProxy>

#include <QFormLayout>

TrashSettingsPage::TrashSettingsPage(QWidget* parent) :
        SettingsPageBase(parent)
{
    QFormLayout* topLayout = new QFormLayout(this);

    m_proxy = new KCModuleProxy(QStringLiteral("kcmtrash"));
    topLayout->addRow(m_proxy);

    loadSettings();

    connect(m_proxy, static_cast<void(KCModuleProxy::*)(bool)>(&KCModuleProxy::changed), this, &TrashSettingsPage::changed);
}

TrashSettingsPage::~TrashSettingsPage()
{
}

void TrashSettingsPage::applySettings()
{
    m_proxy->save();
}

void TrashSettingsPage::restoreDefaults()
{
    m_proxy->defaults();
}

void TrashSettingsPage::loadSettings()
{
    m_proxy->load();
}

