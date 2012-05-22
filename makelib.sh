target=$1
if [ ! -d $target ] 
then
	echo "target "$1" doesn't existst"
	exit 1
fi
echo Build: $target
for x in $(find $target | grep \\.o | grep -iv test); do
	ar rcsv lib/$target/libsambag.a $x
done
