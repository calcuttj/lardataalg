////////////////////////////////////////////////////////////////////////
// \file BoxElectricField.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/BoxElectricField.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Table.h"

#include <algorithm>

namespace detinfo {

  //--------------------------------------------------------------------
  BoxElectricField::BoxElectricField(fhicl::ParameterSet const& pset)
  {
    fhicl::Table<Config> const config{pset};

    auto const lower = config().LowerCorner();
    auto const upper = config().UpperCorner();
    auto const field = config().FieldVector();

    // Store per-axis min/max so the two corners can be given in any order.
    for (std::size_t i = 0; i < 3; ++i) {
      fMin[i] = std::min(lower[i], upper[i]);
      fMax[i] = std::max(lower[i], upper[i]);
    }

    fField = TVector3(field[0], field[1], field[2]);
  }

  //--------------------------------------------------------------------
  TVector3 BoxElectricField::Efield(TVector3 const& point) const
  {
    double const coords[3] = {point.X(), point.Y(), point.Z()};
    for (std::size_t i = 0; i < 3; ++i) {
      if (coords[i] < fMin[i] || coords[i] > fMax[i]) return TVector3(0., 0., 0.);
    }
    return fField;
  }

} // namespace detinfo
