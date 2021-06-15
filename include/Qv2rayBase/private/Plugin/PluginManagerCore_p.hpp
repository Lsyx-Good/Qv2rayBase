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

// ************************ WARNING ************************
//
// This file is NOT part of the Qv2rayBase API.
// It may change at any time without notice, or even be removed.
// USE IT AT YOUR OWN RISK
//
// ************************ WARNING ************************

#pragma once

#include "QvPluginInterface.hpp"

#include <QHash>
#include <QPluginLoader>

namespace Qv2rayBase::Plugin
{
    struct PluginInfo
    {
        QString libraryPath;
        QPluginLoader *loader;
        Qv2rayPlugin::Qv2rayInterface *pinterface;
        Q_ALWAYS_INLINE Qv2rayPlugin::QvPluginMetadata metadata() const
        {
            return pinterface->GetMetadata();
        }
        Q_ALWAYS_INLINE PluginId id() const
        {
            return metadata().InternalID;
        }
        Q_ALWAYS_INLINE bool hasComponent(Qv2rayPlugin::QV2RAY_PLUGIN_COMPONENT_TYPE t) const
        {
            return metadata().Components.contains(t);
        }
    };

    class PluginManagerCorePrivate
    {
      public:
        QHash<PluginId, PluginInfo> plugins;
    };

} // namespace Qv2rayBase::Plugin
