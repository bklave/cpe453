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

echo "minls /Hello/src"
./minls $IMAGE /Hello/src
echo ""



