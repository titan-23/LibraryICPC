g++ -I./../../akakoilib -std=c++20 math/gen.cpp -o gen
g++ -I./../../akakoilib -std=c++20 math/coreect.cpp -o correct
g++ -I./../../akakoilib -std=c++20 math/arbitrary_conv.cpp -o a.out

./gen > in
./correct < in > out.txt
./a.out < in > out2.txt
diff out.txt out2.txt
