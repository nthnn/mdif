rm -rf dist
mkdir -p dist \
    dist/mdif_1.0.2-1_amd64/DEBIAN \
    dist/mdif_1.0.2-1_amd64/usr/local/bin/ \
    dist/mdif_1.0.2-1_amd64/usr/share/applications/ \
    dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/

cd tools/mdif_png && ./build.sh && cd ../..
cd tools/mdif_jpg && ./build.sh && cd ../..
cd tools/mdif_viewer_linux && ./build.sh && cd ../..

cd tools/mdif_png && ./build.sh && cd ../..
cd tools/mdif_viewer_linux && ./build.sh && cd ../..

cp tools/mdif_viewer_linux/build/mdif_viewer dist/mdif_1.0.2-1_amd64/usr/local/bin/mdif_viewer
cp dist/mdif_jpg dist/mdif_1.0.2-1_amd64/usr/local/bin/
cp dist/mdif_png dist/mdif_1.0.2-1_amd64/usr/local/bin/

touch dist/mdif_1.0.2-1_amd64/DEBIAN/control
echo "Package: MDIF" >> dist/mdif_1.0.2-1_amd64/DEBIAN/control
echo "Version: 1.0.2" >> dist/mdif_1.0.2-1_amd64/DEBIAN/control
echo "Architecture: amd64" >> dist/mdif_1.0.2-1_amd64/DEBIAN/control
echo "Maintainer: Nathanne Isip <nathanneisip@gmail.com>" >> dist/mdif_1.0.2-1_amd64/DEBIAN/control
echo "Description: Photo viewer for MDIF (Minimal Data Image Format) file." >> dist/mdif_1.0.2-1_amd64/DEBIAN/control

touch dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "[Desktop Entry]" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Name=MDIF Viewer" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Comment=Photo viewer for MDIF (Minimal Data Image Format) file." >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Exec=/usr/local/bin/mdif_viewer %f" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Icon=mdif_viewer" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Terminal=false" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "Type=Application" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop
echo "MimeType=application/x-mdif" >> dist/mdif_1.0.2-1_amd64/usr/share/applications/mdif_viewer.desktop

touch dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "<mime-info xmlns=\"http://www.freedesktop.org/standards/shared-mime-info\">" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "    <mime-type type=\"application/x-mdif\">" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "        <comment>Minimal Data Image Format</comment>" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "        <glob pattern=\"*.mdif\"/>" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "    </mime-type>" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml
echo "</mime-info>" >> dist/mdif_1.0.2-1_amd64/usr/share/mime/packages/mdif.xml

dpkg-deb --build dist/mdif_1.0.2-1_amd64
./clean.sh