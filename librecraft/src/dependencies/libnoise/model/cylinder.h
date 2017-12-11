// cylinder.h
//
// Copyright 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef NOISE_MODEL_CYLINDER_H
#define NOISE_MODEL_CYLINDER_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "../../libnoise/module/modulebase.h"

namespace noise {

namespace model {

/// @addtogroup libnoise
/// @{

/// @defgroup models Models
/// @addtogroup models
/// @{

/// Model that defines the surface of a cylinder.
///
/// @image html modelcylinder.png
///
/// This model returns an output value from a noise module given the
/// coordinates of an input value located on the surface of a cylinder.
///
/// To generate an output value, pass the (angle, height) coordinates of
/// an input value to the GetValue() method.
///
/// This model is useful for creating:
/// - seamless textures that can be mapped onto a cylinder
///
/// This cylinder has a radius of 1.0 unit and has infinite height.  It is
/// oriented along the @a y axis.  Its center is located at the origin.
class Cylinder
{

public:
  /// Constructor.
  Cylinder();

  /// Constructor
  ///
  /// @param module The noise module that is used to generate the output
  /// values.
  Cylinder(const module::Module& module);

  /// Returns the noise module that is used to generate the output
  /// values.
  ///
  /// @returns A reference to the noise module.
  ///
  /// @pre A noise module was passed to the SetModule() method.
  const module::Module& GetModule() const
  {
    assert(m_pModule != NULL);
    return *m_pModule;
  }

  /// Returns the output value from the noise module given the
  /// (angle, height) coordinates of the specified input value located
  /// on the surface of the cylinder.
  ///
  /// @param angle The angle around the cylinder's center, in degrees.
  /// @param height The height along the @a y axis.
  ///
  /// @returns The output value from the noise module.
  ///
  /// @pre A noise module was passed to the SetModule() method.
  ///
  /// This output value is generated by the noise module passed to the
  /// SetModule() method.
  ///
  /// This cylinder has a radius of 1.0 unit and has infinite height.
  /// It is oriented along the @a y axis.  Its center is located at the
  /// origin.
  double GetValue(double angle, double height) const;

  /// Sets the noise module that is used to generate the output values.
  ///
  /// @param module The noise module that is used to generate the output
  /// values.
  ///
  /// This noise module must exist for the lifetime of this object,
  /// until you pass a new noise module to this method.
  void SetModule(const module::Module& module) { m_pModule = &module; }

private:
  /// A pointer to the noise module used to generate the output values.
  const module::Module* m_pModule;
};

/// @}

/// @}
}
}

#endif
