#!/bin/bash
cd ~/flohofwoe.www
echo "Building web page..."
jekyll
echo "Copying source files..."
srcdir=~/nebula3/code/tools/emsctest
for file in ~/nebula3/code/foundation/app/phasedapplication.cc $srcdir/*/*.cc $srcdir/*/*.h ; do
	expand -t 4 $file > _includes/$(basename $file)
done

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

