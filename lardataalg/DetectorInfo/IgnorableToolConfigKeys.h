////////////////////////////////////////////////////////////////////////
// \file IgnorableToolConfigKeys.h
//
// \brief Config keys a tool-loaded provider should tolerate when validating
//        its own fhicl::Table<Config>.
//
// Extends lar::IgnorableProviderConfigKeys() (art service keys) with the
// "tool_type" key that art::make_tool leaves in the link parameter set, so
// every concrete IDistortion link can validate with a single shared set
// instead of rebuilding it.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_IGNORABLETOOLCONFIGKEYS_H
#define LARDATAALG_DETINFO_IGNORABLETOOLCONFIGKEYS_H

#include "larcorealg/CoreUtils/ProviderUtil.h" // lar::IgnorableProviderConfigKeys()

#include <set>
#include <string>

namespace detinfo {

  /// Ignorable config keys for a provider constructed via art::make_tool:
  /// the art service keys plus "tool_type".
  inline std::set<std::string> const& IgnorableToolConfigKeys()
  {
    static std::set<std::string> const ignorable = [] {
      std::set<std::string> keys = lar::IgnorableProviderConfigKeys();
      keys.insert("tool_type"); // added by art::make_tool
      return keys;
    }();
    return ignorable;
  }

} // namespace detinfo

#endif // LARDATAALG_DETINFO_IGNORABLETOOLCONFIGKEYS_H
