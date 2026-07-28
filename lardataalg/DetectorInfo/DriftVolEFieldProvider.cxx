////////////////////////////////////////////////////////////////////////
// \file DriftVolEFieldProvider.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/DriftVolEFieldProvider.h"

#include "larcorealg/CoreUtils/ProviderUtil.h" // lar::IgnorableProviderConfigKeys()

#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Table.h"

#include <algorithm>
#include <cctype>

namespace {
  // One electrode: its position along the drift axis and whether it is an anode.
  struct Electrode {
    double pos;
    bool isAnode;
  };
}

namespace detinfo {

  //--------------------------------------------------------------------
  DriftVolEFieldProvider::DriftVolEFieldProvider(fhicl::ParameterSet const& pset)
  {
    fhicl::Table<Config> const config{pset, lar::IgnorableProviderConfigKeys()};

    // Cyclic permutation mapping canonical (drift, Y, Z) onto world axes.
    std::string const axis = config().DriftAxis();
    char const a = axis.size() == 1 ? std::tolower(static_cast<unsigned char>(axis[0])) : '\0';
    switch (a) {
    case 'x': fDriftAxis = 0; fYAxis = 1; fZAxis = 2; break;
    case 'y': fDriftAxis = 1; fYAxis = 2; fZAxis = 0; break;
    case 'z': fDriftAxis = 2; fYAxis = 0; fZAxis = 1; break;
    default:
      throw cet::exception("DriftVolEFieldProvider")
        << "DriftAxis must be \"x\", \"y\", or \"z\"; got '" << axis << "'\n";
    }

    auto const extentY = config().ExtentY();
    auto const extentZ = config().ExtentZ();
    fYmin = std::min(extentY[0], extentY[1]);
    fYmax = std::max(extentY[0], extentY[1]);
    fZmin = std::min(extentZ[0], extentZ[1]);
    fZmax = std::max(extentZ[0], extentZ[1]);

    double const magnitude = config().FieldMagnitude();

    // Collect and sort all electrodes along the drift axis.
    std::vector<Electrode> electrodes;
    for (double const p : config().Anodes())
      electrodes.push_back({p, true});
    for (double const p : config().Cathodes())
      electrodes.push_back({p, false});

    if (electrodes.size() < 2) {
      throw cet::exception("DriftVolEFieldProvider")
        << "Need at least one anode and one cathode to form a drift volume.\n";
    }

    std::sort(electrodes.begin(), electrodes.end(), [](Electrode const& e1, Electrode const& e2) {
      return e1.pos < e2.pos;
    });

    fDriftMin = electrodes.front().pos;
    fDriftMax = electrodes.back().pos;

    // Each consecutive pair bounds a drift volume; the electrodes must
    // strictly alternate so the whole extent is tiled with no gaps.
    for (std::size_t i = 0; i + 1 < electrodes.size(); ++i) {
      Electrode const& lo = electrodes[i];
      Electrode const& hi = electrodes[i + 1];

      if (lo.isAnode == hi.isAnode) {
        throw cet::exception("DriftVolEFieldProvider")
          << "Electrodes must strictly alternate anode/cathode along the drift axis; found two "
          << (lo.isAnode ? "anodes" : "cathodes") << " at " << lo.pos << " and " << hi.pos
          << " with no opposite electrode between them.\n";
      }
      if (lo.pos == hi.pos) {
        throw cet::exception("DriftVolEFieldProvider")
          << "Coincident electrodes at " << lo.pos << " define a zero-width drift volume.\n";
      }

      // Field points from cathode toward anode along the drift axis.
      double const component = hi.isAnode ? magnitude : -magnitude;
      double comp[3] = {0., 0., 0.};
      comp[fDriftAxis] = component;

      fVolumes.push_back({lo.pos, hi.pos, TVector3(comp[0], comp[1], comp[2])});
    }
  }

  //--------------------------------------------------------------------
  TVector3 DriftVolEFieldProvider::Efield(TVector3 const& point) const
  {
    double const w[3] = {point.X(), point.Y(), point.Z()};

    // Outside the transverse extents or beyond the outermost electrodes
    // along the drift axis -> outside the active volume -> no field.
    if (w[fYAxis] < fYmin || w[fYAxis] > fYmax) return TVector3(0., 0., 0.);
    if (w[fZAxis] < fZmin || w[fZAxis] > fZmax) return TVector3(0., 0., 0.);
    double const d = w[fDriftAxis];
    if (d < fDriftMin || d > fDriftMax) return TVector3(0., 0., 0.);

    for (auto const& vol : fVolumes) {
      if (d >= vol.lo && d <= vol.hi) return vol.field;
    }

    // Inside the active volume but not enclosed by any drift volume: this
    // should be impossible for a validated (alternating) configuration.
    throw cet::exception("DriftVolEFieldProvider")
      << "Point at drift coordinate " << d
      << " is inside the active volume but not within any drift volume.\n";

    return TVector3(0., 0., 0.); // unreachable; silences missing-return warnings
  }

} // namespace detinfo
