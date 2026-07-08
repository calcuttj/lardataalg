////////////////////////////////////////////////////////////////////////
// \file ElectricFieldProviderFactory.h
//
// \brief Hand-written factory selecting an IElectricFieldProvider from a
//        fhicl::ParameterSet via its "ProviderType" key.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_ELECTRICFIELDPROVIDERFACTORY_H
#define LARDATAALG_DETINFO_ELECTRICFIELDPROVIDERFACTORY_H

#include "lardataalg/DetectorInfo/IElectricFieldProvider.h"

#include <memory>

namespace fhicl {
  class ParameterSet;
}

namespace detinfo {

  /// Creates the concrete IElectricFieldProvider selected by
  /// pset.get<std::string>("ProviderType"). Throws if unknown.
  std::unique_ptr<IElectricFieldProvider> makeElectricFieldProvider(
    fhicl::ParameterSet const& pset);

} // namespace detinfo

#endif // LARDATAALG_DETINFO_ELECTRICFIELDPROVIDERFACTORY_H
