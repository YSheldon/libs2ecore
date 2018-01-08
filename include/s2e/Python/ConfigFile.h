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

///
/// \file ConfigFile.h
/// \brief Wrap the \c ConfigFile class to make it usable within the Python interpreter.
///

#ifndef PY_CONFIG_H_
#define PY_CONFIG_H_

#include <boost/python/list.hpp>
#include <boost/python/object.hpp>

#include <s2e/ConfigFile.h>

namespace py = boost::python;

namespace s2e {

struct PluginInfo;

namespace python {

///
/// \brief A Python wrapper around a \c ConfigFile.
///
/// The wrapped config file is read-only. Each wrapped config file is specific to a particular plugin. This is done so
/// that access to table values is more "Pythonic".
///
class PythonConfigFile {
private:
    /// The wrapped config file
    ConfigFile *const m_cfg;

    ///
    /// \brief The prefix for accessing a plugin-specific config value.
    ///
    /// This will be of the form:
    ///
    /// \code
    ///   pluginsConfig['PLUGIN_CONFIG_KEY'].
    /// \endcode
    ///
    /// where \c PLUGIN_CONFIG_KEY is the plugin's config key.
    ///
    const std::string m_keyPrefix;

public:
    /// Wrap a \c ConfigFile for a specific plugin.
    PythonConfigFile(ConfigFile *const cfg, const PluginInfo *pi);

    ///
    /// \brief Return a value from the config file.
    ///
    /// Returns \c None if the given key does not exist.
    ///
    py::object get(const std::string &name);

    ///
    /// \brief Return a list of string keys for a given table.
    ///
    /// Returns \c None if \c name is invalid.
    ///
    py::object keys(const std::string &name) const;
};

} // namespace python
} // namespace s2e

#endif
