# NNG HomeWork - C++ Duplicate House Number Detection

	This C++ console application processes a street network data file and identifies duplicated house numbers.  
	A house number is considered duplicated if it appears in multiple segments of the same street based on the parity scheme (O = odd, E = even, M = mixed).


# How to Use
	Make sure network.mid (or a test file like network_dummy.txt) is present in the same folder!

1. Clone the repository
	open bash
	git clone https://github.com/Bonagrin/NNGHomeWork.git
	cd NNGHomeWork

2. Build the code using g++
	g++ HomeWork.cpp -o housechecker
	
3. Run the program
	./housechecker