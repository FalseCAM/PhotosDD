/* (C) 2011 FalseCAM
 You should have received a copy of the GNU Lesser General Public License
 along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "photosdd/photosdd.h"
#include <QString>
#include <QMessageBox>
#include <QTranslator>
#include "photosdd/downloader.h"
#include "photosdd/pluginloader.h"

#include "photosdd/fileutils.h"

PhotosDD::PhotosDD() {
}

PhotosDD::~PhotosDD() {
	PluginLoader::getInstance().destroy();
}

/*
 Downloads a file from url to destination directory
 */
bool PhotosDD::download(QString url, QString dir) {
	qDebug("[PhotosDD] downloads %s to %s", qPrintable(url), qPrintable(dir));
	return Downloader::download(url, dir);
}

/*
 Shows a dialog to config (activate/deactivate) plugins
 */
void PhotosDD::configPlugins(QWidget* parent = 0) {
	PluginsDialog pluginsDialog(parent);
	pluginsDialog.exec();
}

QList<PhotosDDPlugin*> PhotosDD::getPlugins() {
	return PluginLoader::getInstance().getActivatedPlugins();
}

/*
 removes files containing in dir and dir then.
 */
bool PhotosDD::removeDir(const QString &dirName) {
	return FileUtils::removeDir(dirName);
}

/*
 About box to show information about PhotosDD library
 */
void PhotosDD::about(QWidget* parent = 0) {
	QMessageBox aboutBox;
	aboutBox.setWindowIcon(PhotosDD::applicationIcon());
	aboutBox.about(
			parent,
			("About ") + PhotosDD::applicationName(),
			("<p align=\"right\"><h1>") + PhotosDD::applicationName()
					+ ("</h1></p>")
					+ ("<p align=\"left\"><font size=\"2\" color=\"grey\">Version ")
					+ PhotosDD::applicationVersion() + ("</font>")
					+ ("<p align=\"left\">(C) 2011 ")
					+ PhotosDD::organizationName() + ("</p>")
					+ ("<p><a href=\"" + PhotosDD::applicationDomain() + "\">")
					+ ("Homepage</a></p>") + ("<br/><br/><p>")
					+ ("English translation: FalseCAM") + ("</p>"));
}
