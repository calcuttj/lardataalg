////////////////////////////////////////////////////////////////////////
// \file NullDistortion.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/NullDistortion.h"

#include "lardataalg/DetectorInfo/IgnorableToolConfigKeys.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Table.h"

namespace detinfo {

  //--------------------------------------------------------------------
  NullDistortion::NullDistortion(fhicl::ParameterSet const& pset)
  {
    // Validate the parameter set (framework + tool_type keys are ignored).
    fhicl::Table<Config> const config{pset, detinfo::IgnorableToolConfigKeys()};
  }

} // namespace detinfo
