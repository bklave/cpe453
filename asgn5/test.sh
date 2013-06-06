IMAGE="images/TestImage"
P_IMAGE="images/Partitioned"

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

echo "minls src"
./minls $IMAGE src
echo ""

echo "minls src/system"
./minls $IMAGE src/system
echo  ""


echo "minls (partitioned)"
./minls $P_IMAGE
echo ""

