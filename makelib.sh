target=Debug/
echo Build: $target
for x in $(find $target | grep \\.o | grep -iv test); do
	ar rcsv lib/$target/libsambag.a $x
done
