/**********************************************************************
  Painter - drawing spheres and cylinders in a GLWidget

  Copyright (C) 2007 Benoit Jacob

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.sourceforge.net/>

  Some code is based on Open Babel
  For more information, see <http://openbabel.sourceforge.net/>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#ifndef __PAINTER_H
#define __PAINTER_H

#include <avogadro/global.h>
#include <eigen/vector.h>
#include <QString>
#include <QPoint>

namespace Avogadro {

  class A_EXPORT GLWidget;
  class PainterPrivate;
  class A_EXPORT Painter
  {
    public:
      Painter();
      ~Painter();

      void setGLWidget( GLWidget * widget );
      void setGlobalQualitySetting( int globalQualitySetting );
      int globalQualitySetting() const;
      void initialize( GLWidget * widget, int globalQualitySetting );

      void drawSphere( const Eigen::Vector3d & center, double radius, int detailLevel ) const;
      void drawSphere( const Eigen::Vector3d & center, double radius ) const;

      void drawCylinder( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
          double radius, int detailLevel ) const;
      void drawCylinder( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
          double radius) const;

      void drawMultiCylinder( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
          double radius, int order, double shift, int detailLevel ) const;
      void drawMultiCylinder( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
          double radius, int order, double shift ) const;

      int drawText( int x, int y, const QString &string ) const;
      int drawText( const QPoint& pos, const QString &string ) const;
      int drawText( const Eigen::Vector3d & pos, const QString &string ) const;
      void beginText() const;
      void endText() const;

    private:
      PainterPrivate * const d;
  };

} // end namespace Avogadro

#endif // _PAINTER_H
