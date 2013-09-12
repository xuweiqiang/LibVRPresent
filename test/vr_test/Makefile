vr_test:  main.cpp
	#PC
	#g++ -o vr_test main.cpp -I. -ldl -DTESTPROGRAM

	#ST
	#/usr/local/x-tools/arm-stm-linux-gnueabi/bin/arm-stm-linux-gnueabi-g++ -o vr_test main.cpp -I. -ldl -DTESTPROGRAM -DST

	#PXA
	/usr/local/arm-linux-4.1.1/bin/arm-iwmmxt-linux-gnueabi-g++ -o vr_test main.cpp -I. -ldl -DTESTPROGRAM -DST

clean:
	rm vr_test
