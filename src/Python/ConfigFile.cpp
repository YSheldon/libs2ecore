/*
 * S2E Selective Symbolic Execution Framework
 *
 * Copyright (c) 2016, Dependable Systems Laboratory, EPFL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Dependable Systems Laboratory, EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE DEPENDABLE SYSTEMS LABORATORY, EPFL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <s2e/Plugin.h>
#include <s2e/Python/ConfigFile.h>

#include "Utils.h"

namespace s2e {
namespace python {

PythonConfigFile::PythonConfigFile(ConfigFile *const cfg, const PluginInfo *pi)
    : m_cfg(cfg), m_keyPrefix("pluginsConfig['" + pi->configKey + "'].") {
}

py::object PythonConfigFile::get(const std::string &name) {
    // Be silent
    bool wasSilent = m_cfg->isSilent();
    m_cfg->setSilent(true);

    bool ok;

    // XXX is string concat the most efficient way to do this?
    std::string key = m_keyPrefix + name;

    // Because Python is dynamically typed, this method can return any value. This method will simply try all the
    // different types until a valid return value is created. If a valid return value cannot be found, return None

    // Try to get a bool
    bool boolRes = m_cfg->getBool(key, false, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(boolRes);
    }

    // Try to get an int
    int64_t intRes = m_cfg->getInt(key, 0, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(intRes);
    }

    // Try to get a double
    double doubleRes = m_cfg->getDouble(key, 0, &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return py::object(doubleRes);
    }

    // Try to get a string
    std::string stringRes = m_cfg->getString(key, "", &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);

        return py::object(stringRes);
    }

    // Try to get a string list
    ConfigFile::string_list stringListRes = m_cfg->getStringList(key, ConfigFile::string_list(), &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return toList(stringListRes);
    }

    // Try to get an integer list
    ConfigFile::integer_list intListRes = m_cfg->getIntegerList(key, ConfigFile::integer_list(), &ok);
    if (ok) {
        m_cfg->setSilent(wasSilent);
        return toList(intListRes);
    }

    m_cfg->setSilent(wasSilent);

    // Return None
    return py::object();
}

py::object PythonConfigFile::keys(const std::string &name) const {
    bool ok;

    ConfigFile::string_list keyList = m_cfg->getListKeys(name, &ok);
    if (ok) {
        return toList(keyList);
    }

    // Return None
    return py::object();
}

} // namespace python
} // namespace s2e
