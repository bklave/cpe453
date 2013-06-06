IMAGE="images/TestImage"

echo "Cleaning..."
make clean
echo ""

echo "Recompiling..."
make
echo ""

echo "minls"
./minls $IMAGE
echo ""

echo "minls /Hello"
./minls $IMAGE /Hello
echo ""

echo "minls /src"
./minls $IMAGE /src
echo ""

echo "minls /"
./minls $IMAGE /
echo ""

echo "minsls"

