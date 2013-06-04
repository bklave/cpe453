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

echo "minls -v"
./minls -v $IMAGE
echo ""

echo "minls -p"
./minls -p $IMAGE
echo ""

echo "minls -p 10"
./minls -p 10 $IMAGE
echo ""

echo "minls -p 10 -s"
./minls -p 10 -s $IMAGE
echo ""

echo "minls -p 10 -s 100"
./minls -p 10 -s 100 $IMAGE
echo ""




