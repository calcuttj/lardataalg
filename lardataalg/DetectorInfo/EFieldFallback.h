////////////////////////////////////////////////////////////////////////
// \file EFieldFallback.h
//
// \brief Helpers for the uniform electric-field fallback used by
//        DetectorProperties implementations when no IElectricFieldProvider
//        is injected: locate the active LAr volume(s) and return a uniform
//        field of a given magnitude inside them (zero outside).
//
// Art-free: depends only on larcorealg geometry + geo_vectors.
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_EFIELDFALLBACK_H
#define LARDATAALG_DETINFO_EFIELDFALLBACK_H

#include "larcorealg/Geometry/BoxBoundedGeo.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

#include <vector>

namespace geo {
  class GeometryCore;
}

namespace detinfo {

  /// Returns one active-LAr box per cryostat (union of all TPC active bounding
  /// boxes). Mirrors larg4::ISTPC::extractActiveLArVolume.
  std::vector<geo::BoxBoundedGeo> extractActiveVolumes(geo::GeometryCore const& geom);

  /// Uniform-field fallback: a vector of magnitude `mag` [kV/cm] along the
  /// containing TPC's drift direction if `point` is inside one of
  /// `activeVolumes`, else {0,0,0}.
  geo::Vector_t uniformFallbackEField(geo::GeometryCore const& geom,
                                      std::vector<geo::BoxBoundedGeo> const& activeVolumes,
                                      double mag,
                                      geo::Point_t const& point);

} // namespace detinfo

#endif // LARDATAALG_DETINFO_EFIELDFALLBACK_H
