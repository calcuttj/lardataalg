////////////////////////////////////////////////////////////////////////
// \file BoxElectricField.h
//
// \brief IElectricFieldProvider returning a uniform field vector inside an
//        axis-aligned box (two opposite corners) and {0,0,0} outside.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_BOXELECTRICFIELD_H
#define LARDATAALG_DETINFO_BOXELECTRICFIELD_H

#include "lardataalg/DetectorInfo/IElectricFieldProvider.h"

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"

#include "TVector3.h"

#include <array>
#include <string>

namespace fhicl {
  class ParameterSet;
}

namespace detinfo {

  class BoxElectricField : public IElectricFieldProvider {
  public:
    struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;

      fhicl::Atom<std::string> ProviderType{Name("ProviderType"),
                                            Comment("electric field provider selector")};
      fhicl::Sequence<double, 3> LowerCorner{
        Name("LowerCorner"),
        Comment("one corner of the axis-aligned box [cm]: {x, y, z}")};
      fhicl::Sequence<double, 3> UpperCorner{
        Name("UpperCorner"),
        Comment("opposite corner of the axis-aligned box [cm]: {x, y, z}")};
      fhicl::Sequence<double, 3> FieldVector{
        Name("FieldVector"),
        Comment("electric field vector inside the box [kV/cm]: {Ex, Ey, Ez}")};
    };

    explicit BoxElectricField(fhicl::ParameterSet const& pset);

    /// Returns the field vector if point is inside the box (inclusive),
    /// else {0,0,0}.
    TVector3 Efield(TVector3 const& point) const override;

  private:
    std::array<double, 3> fMin;   ///< per-axis lower bound [cm]
    std::array<double, 3> fMax;   ///< per-axis upper bound [cm]
    TVector3 fField;              ///< field vector inside the box [kV/cm]

  }; // class BoxElectricField
} // namespace detinfo

#endif // LARDATAALG_DETINFO_BOXELECTRICFIELD_H
