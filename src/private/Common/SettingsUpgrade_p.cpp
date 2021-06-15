//  Qv2rayBase, the modular feature-rich infrastructure library for Qv2ray.
//  Copyright (C) 2021 Moody and relavent Qv2ray contributors.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "private/Common/SettingsUpgrade_p.hpp"

#include "Common/Settings.hpp"
#include "Qv2rayBaseLibrary.hpp"

#define QV_MODULE_NAME "SettingsUpgrade"

namespace Qv2rayBase::_private
{
    QJsonObject UpgradeConfigInc(int fromVersion, const QJsonObject &original)
    {
        auto root = original;
        switch (fromVersion)
        {
            default:
            {
                Qv2rayBaseLibrary::Warn(QObject::tr("Configuration Upgrade Failed"),
                                        QObject::tr("Unsupported config version number: ") + QString::number(fromVersion) + NEWLINE + NEWLINE +
                                            QObject::tr("Please go to https://github.com/Qv2ray/Qv2ray/issues to check for related announcements."));
                QvLog() << "The configuration version of your old Qv2ray installation is out-of-date and that version is not supported anymore.";
                QvLog() << "Please try to update to an intermediate version of Qv2ray first.";
                exit(1);
            }
        }
        root["config_version"] = root["config_version"].toInt() + 1;
        return root;
    }

    QJsonObject MigrateSettings(const QJsonObject &original)
    {
        const auto fileVersion = original["config_version"].toInt(QV2RAY_SETTINGS_VERSION);
        QvLog() << "Migrating config from version" << fileVersion;

        auto root = original;
        for (int i = fileVersion; i < QV2RAY_SETTINGS_VERSION; i++)
            root = UpgradeConfigInc(i, root);

        return root;
    }
} // namespace Qv2rayBase::_private
