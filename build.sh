#!/bin/bash
cd ~/flohofwoe.www
echo "Building web page..."
jekyll
echo "Copying asset files..."
mkdir _site/demos
rsync -a ~/emsctest/cdndata _site/demos
echo "Gzipping..."
for file in ~/emsctest/*.html ; do
	gzip -c $file > _site/demos/$(basename $file).gz
done
for file in ~/emsctest/*.mem ; do
	gzip -c $file > _site/demos/$(basename $file).gz
done
for file in ~/emsctest/*.js ; do
	gzip -c $file > _site/demos/$(basename $file).gz
done
echo "Copy htaccess file..."
cp _htaccess _site/demos/.htaccess
echo "Done."

