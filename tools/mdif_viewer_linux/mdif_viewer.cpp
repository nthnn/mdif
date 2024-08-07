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
#include "mdif.h"

class MDIFViewer : public QMainWindow {
public:
    MDIFViewer(const QString& filename) {
        mdif_error_t result = mdif_read(
            filename.toUtf8().constData(),
            &image
        );

        if(result != MDIF_ERROR_NONE) {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr(mdif_error_message(result))
            );
            exit(-1);
        }

        QWidget* centralWidget = new QWidget(this);

        setWindowTitle(tr("MDIF Viewer"));
        setCentralWidget(centralWidget);
        setFixedSize(image.width, image.height);

        connect(this, &MDIFViewer::destroyed, [this]() {
            mdif_free(&image);
        });
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        QImage img(image.width, image.height, QImage::Format_ARGB32);

        for(int y = 0; y < image.height; ++y) {
            for(int x = 0; x < image.width; ++x) {
                QRgb color = qRgba(image.red[y * image.width + x], 
                    image.green[y * image.width + x], 
                    image.blue[y * image.width + x], 
                    image.alpha[y * image.width + x]
                );
                img.setPixel(x, y, color);
            }
        }

        painter.drawImage(0, 0, img);
    }

private:
    mdif_t image;
};