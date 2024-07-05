/* 
 * Copyright 2024 Nathanne Isip
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtWidgets>
#include "mdif_viewer.cpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    if(argc < 2) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Error"),
            QObject::tr("Usage: mdif_viewer <filename.mdif>")
        );
        exit(-1);
    }

    const QString mdifFilename = QString::fromUtf8(argv[1]);
    MDIFViewer viewer(mdifFilename);

    viewer.show();
    return app.exec();
}
