#!/bin/bash
cd ~/flohofwoe.www
echo "Building web page..."
jekyll
echo "Copying source files..."
srcdir=~/nebula3/code/tools/emsctest

# copy a few select source files (expand cmd expands tabs to spaces)
for file in ~/nebula3/code/foundation/app/phasedapplication.cc \
			$srcdir/dragons/dragons.cc \
			$srcdir/dragons/dragonsapplication.cc \
			$srcdir/base/emsctestapplication.cc ; do
	expand -t 4 $file > _includes/$(basename $file)
done
rm _includes/terrain*

echo "Copying asset files..."
mkdir _site/demos
rsync -a ~/emsctest/cdndata _site/demos
echo "Gzipping..."

for file in ~/emsctest/*.html ; do
	gzip --best -c $file > _site/demos/$(basename $file).gz
done
# for file in ~/emsctest/*.mem ; do
# 	gzip --best -c $file > _site/demos/$(basename $file).gz
# done
for file in ~/emsctest/*.js ; do
	gzip --best -c $file > _site/demos/$(basename $file).gz
done
cp ~/emsctest/*.pexe _site/demos/
cp ~/emsctest/*.nmf _site/demos/

echo "Copy htaccess file..."
cp _htaccess _site/demos/.htaccess
for dir in _site/oldstuff/* ; do
	cp _htaccess $dir/.htaccess
done

# clean up non-gzip jsmess "exes"
for str in kc85_3 kc85_4 pengo labyrinth cave; do
	rm _site/oldstuff/$str/messkc*.js
done
echo "Done."

