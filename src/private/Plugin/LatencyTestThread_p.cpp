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

#include "private/Plugin/LatencyTestThread_p.hpp"

#include "Common/ProfileHelpers.hpp"
#include "Plugin/LatencyTestHost.hpp"
#include "Plugin/PluginAPIHost.hpp"
#include "Qv2rayBaseLibrary.hpp"

#ifndef QV2RAYBASE_NO_LIBUV
#include <uvw.hpp>
#endif

namespace Qv2rayBase::Plugin
{
    LatencyTestThread::LatencyTestThread(QObject *parent) : QThread(parent)
    {
    }

    void LatencyTestThread::stopLatencyTest()
    {
        isStop = true;
    }

    void LatencyTestThread::pushRequest(const ConnectionId &id, const LatencyTestEngineId &engine)
    {
        if (isStop)
            return;
        std::unique_lock<std::mutex> lockGuard{ m };
        const auto &[protocol, host, port] = GetOutboundInfo(GetOutbound(id, 0));
        requests.emplace_back(Qv2rayPlugin::Latency::LatencyTestRequest{ engine, id, host, port.from });
    }

    void LatencyTestThread::run()
    {
#ifndef QV2RAYBASE_NO_LIBUV
        loop = uvw::Loop::create();
        stopTimer = loop->resource<uvw::TimerHandle>();
        stopTimer->on<uvw::TimerEvent>([this](auto &, auto &handle) { doTest(qobject_cast<Qv2rayBase::Plugin::LatencyTestHost *>(parent()), handle); });
        stopTimer->start(uvw::TimerHandle::Time{ 500 }, uvw::TimerHandle::Time{ 500 });
        loop->run();
#else
        while (true)
        {
            doTest(qobject_cast<Qv2rayBase::Plugin::LatencyTestHost *>(parent()));
            QThread::sleep(1000);
        }
#endif
    }

#ifndef QV2RAYBASE_NO_LIBUV
    void LatencyTestThread::doTest(Qv2rayBase::Plugin::LatencyTestHost *parent, uvw::TimerHandle &handle)
#else
    void LatencyTestThread::doTest(Qv2rayBase::Plugin::LatencyTestHost *parent)
#endif
    {
        if (isStop)
        {
            if (!requests.empty())
                requests.clear();
#ifndef QV2RAYBASE_NO_LIBUV
            int timer_count = 0;
            uv_walk(
                loop->raw(),
                [](uv_handle_t *handle, void *arg)
                {
                    int &counter = *static_cast<int *>(arg);
                    if (uv_is_closing(handle) == 0)
                        counter++;
                },
                &timer_count);
            if (timer_count == 1) // only current timer
            {
                handle.stop();
                handle.close();
                loop->clear();
                loop->close();
                loop->stop();
            }
#else
            return;
#endif
        }
        else
        {
            if (requests.empty())
                return;
            std::unique_lock<std::mutex> lockGuard{ m };
            for (auto &req : requests)
            {
                const auto engineInfo = Qv2rayBaseLibrary::PluginAPIHost()->Latency_GetEngine(req.engine);
                // This is a blocking call
                Qv2rayPlugin::LatencyTestResponse resp;
#ifndef QV2RAYBASE_NO_LIBUV
                if (engineInfo.isAsync)
                    resp = engineInfo.Create()->TestLatencyAsync(loop, req);
                else
#endif
                    resp = engineInfo.Create()->TestLatency(req);
                emit parent->OnLatencyTestCompleted(req.id, resp);
            }
            requests.clear();
        }
    }

} // namespace Qv2rayBase::Plugin
