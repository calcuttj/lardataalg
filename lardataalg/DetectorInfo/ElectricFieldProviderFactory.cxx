////////////////////////////////////////////////////////////////////////
// \file ElectricFieldProviderFactory.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/ElectricFieldProviderFactory.h"
#include "lardataalg/DetectorInfo/BoxElectricField.h"
#include "lardataalg/DetectorInfo/DriftVolEFieldProvider.h"

#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"

#include <string>

namespace detinfo {

  std::unique_ptr<IElectricFieldProvider> makeElectricFieldProvider(
    fhicl::ParameterSet const& pset)
  {
    auto const providerType = pset.get<std::string>("ProviderType");

    if (providerType == "Box") { return std::make_unique<BoxElectricField>(pset); }
    if (providerType == "DriftVol") { return std::make_unique<DriftVolEFieldProvider>(pset); }

    throw cet::exception("ElectricFieldProviderFactory")
      << "Unknown electric field ProviderType: '" << providerType << "'\n";
  }

} // namespace detinfo
