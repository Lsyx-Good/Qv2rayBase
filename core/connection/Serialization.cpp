#include "Serialization.hpp"

#include "ConfigHandler.hpp"
#include "Generation.hpp"

namespace Qv2ray::core::connection
{
    namespace serialization
    {
        QList<std::pair<QString, CONFIGROOT>> ConvertConfigFromString(const QString &link, QString *aliasPrefix, QString *errMessage, QString *newGroup)
        {

            QList<std::pair<QString, CONFIGROOT>> connectionConf;
            if (link.startsWith("vmess://") && link.contains("@"))
            {
                auto conf = vmess_new::Deserialize(link, aliasPrefix, errMessage);
                connectionConf << std::pair{ *aliasPrefix, conf };
            }
            else if (link.startsWith("vless://"))
            {
                auto conf = vless::Deserialize(link, aliasPrefix, errMessage);
                connectionConf << std::pair{ *aliasPrefix, conf };
            }
            else if (link.startsWith("vmess://"))
            {
                auto conf = vmess::Deserialize(link, aliasPrefix, errMessage);
                connectionConf << std::pair{ *aliasPrefix, conf };
            }
            else if (link.startsWith("ss://") && !link.contains("plugin="))
            {
                auto conf = ss::Deserialize(link, aliasPrefix, errMessage);
                connectionConf << std::pair{ *aliasPrefix, conf };
            }
            else if (link.startsWith("ssd://"))
            {
                QStringList errMessageList;
                connectionConf << ssd::Deserialize(link, newGroup, &errMessageList);
                *errMessage = errMessageList.join(NEWLINE);
            }
            else
            {
                const auto config = PluginHost->Outbound_Deserialize(link);
                if (config)
                {
                    const auto &[_alias, _protocol, _outbound, _stream] = *config;
                    CONFIGROOT root;
                    const auto outbound = GenerateOutboundEntry(OUTBOUND_TAG_PROXY, _protocol, OUTBOUNDSETTING(_outbound), _stream);
                    QJsonIO::SetValue(root, outbound, "outbounds", 0);
                    connectionConf << std::pair{ _alias, root };
                }
                else if (errMessage->isEmpty())
                {
                    *errMessage = QObject::tr("Unsupported share link format.");
                }
            }

            return connectionConf;
        }

        const QString ConvertConfigToString(const ConnectionGroupPair &identifier)
        {
            auto alias = GetDisplayName(identifier.connectionId);
            if (IsComplexConfig(identifier.connectionId))
            {
                return QV2RAY_SERIALIZATION_COMPLEX_CONFIG_PLACEHOLDER;
            }
            auto server = ConnectionManager->GetConnectionRoot(identifier.connectionId);
            return ConvertConfigToString(alias, GetDisplayName(identifier.groupId), server);
        }

        const QString ConvertConfigToString(const QString &alias, const QString &groupName, const CONFIGROOT &server)
        {
            const auto outbound = OUTBOUND(server["outbounds"].toArray().first().toObject());
            const auto type = outbound["protocol"].toString();
            const auto settings = outbound["settings"].toObject();
            const auto streamSettings = outbound["streamSettings"].toObject();

            QString sharelink;

            if (type.isEmpty())
            {
                return "";
            }

            if (type == "vmess")
            {
                const auto vmessServer = VMessServerObject(settings["vnext"].toArray().first().toObject());
                const auto transport = StreamSettingsObject(streamSettings);
                sharelink = vmess_new::Serialize(transport, vmessServer, alias);
            }
            else if (type == "shadowsocks")
            {
                auto ssServer = ShadowSocksServerObject(settings["servers"].toArray().first().toObject());
                sharelink = ss::Serialize(ssServer, alias);
            }
            else
            {
                const auto result = PluginHost->Outbound_Serialize(PluginOutboundDescriptor{ alias, type, settings, streamSettings });
                return *result;
            }

            return sharelink;
        }

    } // namespace serialization
} // namespace Qv2ray::core::connection
