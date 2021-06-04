#pragma once

#include <QMap>
#include <QtCore>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <optional>
#include <vector>
// Base support.
#include "Qv2rayFeatures.hpp"
#include "Qv2rayLog.hpp"
// Code Models
#include "Qv2rayBaseApplication.hpp"
#include "models/CoreObjectModels.hpp"
#include "models/QvRuntimeConfig.hpp"
#include "models/QvSafeType.hpp"
#include "models/QvSettingsObject.hpp"
#include "qv2raybase_export.h"

using namespace Qv2ray;
using namespace Qv2ray::base;
using namespace Qv2ray::base::safetype;
using namespace Qv2ray::base::config;
using namespace Qv2ray::base::objects;
using namespace Qv2ray::base::objects::protocol;
using namespace Qv2ray::base::objects::transfer;

#define QV2RAY_BUILD_INFO QString(_QV2RAY_BUILD_INFO_STR_)
#define QV2RAY_BUILD_EXTRA_INFO QString(_QV2RAY_BUILD_EXTRA_INFO_STR_)

// Base folder suffix.
#ifdef QT_DEBUG
#define QV2RAY_CONFIG_DIR_SUFFIX "_debug/"
#define _BOMB_ (static_cast<QObject *>(nullptr)->event(nullptr))
#else
#define _BOMB_
#define QV2RAY_CONFIG_DIR_SUFFIX "/"
#endif

#ifdef Q_OS_WIN
#define QV2RAY_EXECUTABLE_SUFFIX ".exe"
#else
#define QV2RAY_EXECUTABLE_SUFFIX ""
#endif

// Get Configured Config Dir Path
#define QV2RAY_CONFIG_DIR (QvCoreApplication->ConfigPath)
#define QV2RAY_CONFIG_FILE (QV2RAY_CONFIG_DIR + "Qv2ray.conf")
#define QV2RAY_CONNECTIONS_DIR (QV2RAY_CONFIG_DIR + "connections/")
#define QV2RAY_CONFIG_FILE_EXTENSION ".qv2ray.json"
#define QV2RAY_GENERATED_DIR (QV2RAY_CONFIG_DIR + "generated/")

#if !defined(QV2RAY_DEFAULT_VCORE_PATH) && !defined(QV2RAY_DEFAULT_VASSETS_PATH)
#define QV2RAY_DEFAULT_VASSETS_PATH (QV2RAY_CONFIG_DIR + "vcore/")
#define QV2RAY_DEFAULT_VCORE_PATH (QV2RAY_CONFIG_DIR + "vcore/v2ray" QV2RAY_EXECUTABLE_SUFFIX)
#define QV2RAY_DEFAULT_VCTL_PATH (QV2RAY_CONFIG_DIR + "vcore/v2ctl" QV2RAY_EXECUTABLE_SUFFIX)
#elif defined(QV2RAY_DEFAULT_VCORE_PATH) && defined(QV2RAY_DEFAULT_VASSETS_PATH)
// ---- Using user-specified VCore and VAssets path
#else
#error Both QV2RAY_DEFAULT_VCORE_PATH and QV2RAY_DEFAULT_VASSETS_PATH need to be presented when using manually specify the paths.
#endif

#define QSTRN(num) QString::number(num)

#define OUTBOUND_TAG_BLACKHOLE "BLACKHOLE"
#define OUTBOUND_TAG_DIRECT "DIRECT"
#define OUTBOUND_TAG_PROXY "PROXY"
#define OUTBOUND_TAG_FORWARD_PROXY "QV2RAY_FORWARD_PROXY"

#define API_TAG_DEFAULT "QV2RAY_API"
#define API_TAG_INBOUND "QV2RAY_API_INBOUND"

#define QV2RAY_USE_FPROXY_KEY "_QV2RAY_USE_GLOBAL_FORWARD_PROXY_"
