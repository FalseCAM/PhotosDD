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
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

#include "photosdd/fileutils.h"

FileUtils::FileUtils(QObject *parent) :
	QObject(parent) {
}

/*!
 Delete a directory along with all of its contents.

 \param dirName Path of directory to remove.
 \return true on success; false on error.
 */
bool FileUtils::removeDir(const QString &dirName) {
	bool result = true;
	QDir dir(dirName);

	if (dir.exists(dirName)) {
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
			{
				if (info.isDir()) {
					result = removeDir(info.absoluteFilePath());
				} else {
					result = QFile::remove(info.absoluteFilePath());
				}

				if (!result) {
					return result;
				}
			}
		result = dir.rmdir(dirName);
	}

	return result;
}
