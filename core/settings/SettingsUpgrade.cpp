//
// This file handles some important migration
// from old to newer versions of Qv2ray.
//

#include "Qv2rayBase.hpp"
#include "QvHelpers.hpp"

#define QV_MODULE_NAME "SettingsUpgrade"

#define UPGRADELOG(msg) LOG("[" + QSTRN(fromVersion) + "-" + QSTRN(fromVersion + 1) + "] --> " + msg)

namespace Qv2ray
{
    // Private member
    QJsonObject UpgradeConfig_Inc(int fromVersion, const QJsonObject &original)
    {
        auto root = original;
        switch (fromVersion)
        {
            default:
            {
                QvMessageBoxWarn(nullptr, QObject::tr("Configuration Upgrade Failed"),
                                 QObject::tr("Unsupported config version number: ") + QSTRN(fromVersion) + NEWLINE + NEWLINE +
                                     QObject::tr("Please go to https://github.com/Qv2ray/Qv2ray/issues to check for related announcements."));
                LOG("The configuration version of your old Qv2ray installation is out-of-date and that"
                    " version is not supported anymore, please try to update to an intermediate version of Qv2ray first.");
                exit(1);
            }
        }
        root["config_version"] = root["config_version"].toInt() + 1;
        return root;
    }

    // Exported function
    QJsonObject UpgradeSettingsVersion(int fromVersion, int toVersion, const QJsonObject &original)
    {
        auto root = original;
        LOG("Migrating config from version ", fromVersion, "to", toVersion);

        for (int i = fromVersion; i < toVersion; i++)
        {
            root = UpgradeConfig_Inc(i, root);
        }

        return root;
    }
} // namespace Qv2ray
