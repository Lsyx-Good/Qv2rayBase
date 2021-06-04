#pragma once
#include "CoreUtils.hpp"
#include "PluginAPIHost.hpp"
#include "Qv2rayBase.hpp"

#include <QObject>
#include <optional>

namespace Qv2ray::core::kernel
{
    class V2RayKernelInstance;
}

namespace Qv2ray::core::handler
{
    class QV2RAYBASE_EXPORT KernelInstanceHandler : public QObject
    {
        Q_OBJECT
      public:
        explicit KernelInstanceHandler(QObject *parent = nullptr);
        ~KernelInstanceHandler();

        std::optional<QString> StartConnection(const ConnectionGroupPair &id, CONFIGROOT root);
        void StopConnection();
        const ConnectionGroupPair CurrentConnection() const
        {
            return currentId;
        }
        size_t ActivePluginKernelsCount() const
        {
            return activeKernels.size();
        }
        const QMap<QString, PluginIOBoundData> GetCurrentConnectionInboundInfo() const
        {
            return inboundInfo;
        }
        const QStringList GetActiveKernelProtocols() const
        {
            QStringList list;
            for (const auto &[protocol, kernel] : activeKernels)
            {
                list << protocol;
            }
            return list;
        }

      signals:
        void OnConnected(const ConnectionGroupPair &id);
        void OnDisconnected(const ConnectionGroupPair &id);
        void OnCrashed(const ConnectionGroupPair &id, const QString &errMessage);
        void OnStatsDataAvailable(const ConnectionGroupPair &id, const QMap<StatisticsType, QvStatsSpeed> &data);
        void OnKernelLogAvailable(const ConnectionGroupPair &id, const QString &log);

      private slots:
        void OnKernelCrashed_p(const QString &msg);
        void OnPluginKernelLog_p(const QString &log);
        void OnV2RayKernelLog_p(const QString &log);
        void OnV2RayStatsDataRcvd_p(const QMap<StatisticsType, QvStatsSpeed> &data);
        void OnPluginStatsDataRcvd_p(const long uploadSpeed, const long downloadSpeed);

      private:
        void emitLogMessage(const QString &);
        static std::optional<QString> CheckPort(const QMap<QString, PluginIOBoundData> &info, int plugins);

      private:
        qsizetype pluginLogPrefixPadding = 0;
        QMap<QString, int> GetInboundProtocolPorts() const
        {
            QMap<QString, int> result;
            for (const auto &[tag, info] : inboundInfo.toStdMap())
            {
                result[info[IOBOUND::PROTOCOL].toString()] = info[IOBOUND::PORT].toInt();
            }
            return result;
        }

        // Since QMap does not support std::unique_ptr, we use std::map<>
        std::list<std::pair<QString, std::unique_ptr<PluginKernel>>> activeKernels;
        QMap<QString, PluginIOBoundData> inboundInfo;
        Qv2ray::core::kernel::V2RayKernelInstance *vCoreInstance = nullptr;
        ConnectionGroupPair currentId = {};
    };
    inline const KernelInstanceHandler *KernelInstance;
} // namespace Qv2ray::core::handler
