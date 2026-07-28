////////////////////////////////////////////////////////////////////////
// \file NullPositionDistorter.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/NullPositionDistorter.h"

#include "larcorealg/CoreUtils/ProviderUtil.h" // lar::IgnorableProviderConfigKeys()

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Table.h"

namespace detinfo {

  //--------------------------------------------------------------------
  NullPositionDistorter::NullPositionDistorter(fhicl::ParameterSet const& pset)
  {
    // Validate the parameter set (only framework keys are expected/ignored).
    fhicl::Table<Config> const config{pset, lar::IgnorableProviderConfigKeys()};
  }

} // namespace detinfo
