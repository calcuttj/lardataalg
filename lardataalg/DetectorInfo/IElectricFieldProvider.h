////////////////////////////////////////////////////////////////////////
// \file IElectricFieldProvider.h
//
// \brief pure virtual base interface for a position-aware electric field
//
// Modeled on spacecharge::SpaceCharge: a framework-agnostic (art-free),
// pure-virtual base living in its own header/namespace, taking a spatial
// point and returning the electric field there.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_IELECTRICFIELDPROVIDER_H
#define LARDATAALG_DETINFO_IELECTRICFIELDPROVIDER_H

#include "TVector3.h"

namespace detinfo {

  class IElectricFieldProvider {
  public:
    IElectricFieldProvider(IElectricFieldProvider const&) = delete;
    IElectricFieldProvider(IElectricFieldProvider&&) = delete;
    IElectricFieldProvider& operator=(IElectricFieldProvider const&) = delete;
    IElectricFieldProvider& operator=(IElectricFieldProvider&&) = delete;
    virtual ~IElectricFieldProvider() = default;

    /// Returns the electric field vector at the given point, in kV/cm.
    virtual TVector3 Efield(TVector3 const& point) const = 0;

  protected:
    IElectricFieldProvider() = default;

  }; // class IElectricFieldProvider
} // namespace detinfo

#endif // LARDATAALG_DETINFO_IELECTRICFIELDPROVIDER_H
