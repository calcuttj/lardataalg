////////////////////////////////////////////////////////////////////////
// \file EFieldFallback.cxx
////////////////////////////////////////////////////////////////////////

#include "lardataalg/DetectorInfo/EFieldFallback.h"

#include "larcorealg/Geometry/CryostatGeo.h"
#include "larcorealg/Geometry/GeometryCore.h"
#include "larcorealg/Geometry/TPCGeo.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

#include <utility> // std::move

namespace detinfo {

  //--------------------------------------------------------------------
  std::vector<geo::BoxBoundedGeo> extractActiveVolumes(geo::GeometryCore const& geom)
  {
    std::vector<geo::BoxBoundedGeo> activeVolumes;
    activeVolumes.reserve(geom.Ncryostats());
    for (geo::CryostatGeo const& cryo : geom.Iterate<geo::CryostatGeo>()) {
      geo::BoxBoundedGeo box{cryo.TPC(0).ActiveBoundingBox()};
      for (geo::TPCGeo const& tpc : cryo.IterateTPCs())
        box.ExtendToInclude(tpc.ActiveBoundingBox());
      activeVolumes.push_back(std::move(box));
    }
    return activeVolumes;
  }

  //--------------------------------------------------------------------
  TVector3 uniformFallbackEField(geo::GeometryCore const& geom,
                                 std::vector<geo::BoxBoundedGeo> const& activeVolumes,
                                 double const mag,
                                 TVector3 const& point)
  {
    geo::Point_t const gp{point.X(), point.Y(), point.Z()};

    bool inActive = false;
    for (auto const& box : activeVolumes) {
      if (box.ContainsPosition(gp)) {
        inActive = true;
        break;
      }
    }
    if (!inActive) return TVector3(0., 0., 0.);

    geo::TPCGeo const* tpc = geom.PositionToTPCptr(gp);
    geo::Vector_t const dir = tpc ? tpc->DriftDir() : geo::Vector_t{1., 0., 0.};
    return TVector3(mag * dir.X(), mag * dir.Y(), mag * dir.Z());
  }

} // namespace detinfo
