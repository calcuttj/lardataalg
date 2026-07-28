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
  geo::Vector_t uniformFallbackEField(geo::GeometryCore const& geom,
                                      std::vector<geo::BoxBoundedGeo> const& activeVolumes,
                                      double const mag,
                                      geo::Point_t const& point)
  {
    bool inActive = false;
    for (auto const& box : activeVolumes) {
      if (box.ContainsPosition(point)) {
        inActive = true;
        break;
      }
    }
    if (!inActive) return geo::Vector_t(0., 0., 0.);

    geo::TPCGeo const* tpc = geom.PositionToTPCptr(point);
    geo::Vector_t const dir = tpc ? tpc->DriftDir() : geo::Vector_t{1., 0., 0.};
    return geo::Vector_t(mag * dir.X(), mag * dir.Y(), mag * dir.Z());
  }

} // namespace detinfo
