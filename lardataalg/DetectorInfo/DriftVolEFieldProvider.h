////////////////////////////////////////////////////////////////////////
// \file DriftVolEFieldProvider.h
//
// \brief IElectricFieldProvider for an active volume segmented along a
//        drift axis into anode<->cathode drift volumes.
//
// The provider is defined in a canonical frame in which the drift axis is
// local-x and the two transverse directions are local-y and local-z:
//   - Anodes/Cathodes give electrode plane positions along the drift (x);
//   - ExtentY / ExtentZ give the transverse [min,max] bounds;
//   - the drift extent is defined by the outermost electrodes.
// Consecutive electrodes (which must strictly alternate anode/cathode)
// bound a drift volume; inside it the field has the configured magnitude
// and points from the cathode toward the anode along the drift axis.
//
// DriftAxis selects a cyclic permutation mapping the canonical frame onto
// world axes: "x" = identity; "y" maps (drift,Y,Z)->(y,z,x); "z" maps
// (drift,Y,Z)->(z,x,y).
//
// A point outside the active volume (either transverse extent, or beyond
// the outermost electrodes along the drift axis) has field {0,0,0}. A
// point inside the active volume that is not enclosed by any drift volume
// is a configuration error and throws.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_DRIFTVOLEFIELDPROVIDER_H
#define LARDATAALG_DETINFO_DRIFTVOLEFIELDPROVIDER_H

#include "lardataalg/DetectorInfo/IElectricFieldProvider.h"

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"

#include "TVector3.h"

#include <string>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

namespace detinfo {

  class DriftVolEFieldProvider : public IElectricFieldProvider {
  public:
    struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;

      fhicl::Atom<std::string> ProviderType{Name("ProviderType"),
                                            Comment("electric field provider selector")};
      fhicl::Atom<std::string> DriftAxis{
        Name("DriftAxis"),
        Comment("world drift axis / canonical-frame permutation: \"x\", \"y\", or \"z\"")};
      fhicl::Sequence<double, 2> ExtentY{
        Name("ExtentY"),
        Comment("transverse (canonical-y) extent [cm]: {min, max}")};
      fhicl::Sequence<double, 2> ExtentZ{
        Name("ExtentZ"),
        Comment("transverse (canonical-z) extent [cm]: {min, max}")};
      fhicl::Sequence<double> Anodes{
        Name("Anodes"),
        Comment("anode plane positions along the drift axis [cm]")};
      fhicl::Sequence<double> Cathodes{
        Name("Cathodes"),
        Comment("cathode plane positions along the drift axis [cm]")};
      fhicl::Atom<double> FieldMagnitude{
        Name("FieldMagnitude"),
        Comment("field magnitude inside every drift volume [kV/cm]")};
    };

    explicit DriftVolEFieldProvider(fhicl::ParameterSet const& pset);

    /// Returns the enclosing drift-volume field vector, or {0,0,0} outside
    /// the active volume. Throws if a point inside the active volume is not
    /// enclosed by any drift volume.
    TVector3 Efield(TVector3 const& point) const override;

  private:
    /// One drift volume: [lo, hi] along the world drift axis, with a
    /// precomputed field vector pointing from its cathode toward its anode.
    struct DriftVolume {
      double lo;      ///< lower drift-axis bound [cm]
      double hi;      ///< upper drift-axis bound [cm]
      TVector3 field; ///< field vector inside this volume [kV/cm]
    };

    unsigned int fDriftAxis;           ///< world index of the drift axis (0=x,1=y,2=z)
    unsigned int fYAxis;               ///< world index carrying the canonical-y extent
    unsigned int fZAxis;               ///< world index carrying the canonical-z extent
    double fYmin, fYmax;               ///< transverse (canonical-y) bounds [cm]
    double fZmin, fZmax;               ///< transverse (canonical-z) bounds [cm]
    double fDriftMin, fDriftMax;       ///< active drift extent (outermost electrodes) [cm]
    std::vector<DriftVolume> fVolumes; ///< drift volumes along the drift axis

  }; // class DriftVolEFieldProvider
} // namespace detinfo

#endif // LARDATAALG_DETINFO_DRIFTVOLEFIELDPROVIDER_H
