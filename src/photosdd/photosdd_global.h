/* (C) 2011 FalseCAM
    This file is part of PhotosDD.

    PhotosDD is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    PhotosDD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PHOTOSDD_GLOBAL_H
#define PHOTOSDD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PHOTOSDD_LIBRARY)
#  define PHOTOSDDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PHOTOSDDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PHOTOSDD_GLOBAL_H
