vr_test:  render_video_cidana.cpp
	#PC
	~/golan/arm-2010q1/bin/arm-none-linux-gnueabi-g++ -o vr_test render_video_cidana.cpp -I. -ldl -DTESTPROGRAM -D_GNU_LINUX ./libstg.a -lrt

	#ST
	#/usr/local/x-tools/arm-stm-linux-gnueabi/bin/arm-stm-linux-gnueabi-g++ -o vr_test main.cpp -I. -ldl -DTESTPROGRAM -DST

	#PXA
	#/usr/local/arm-linux-4.1.1/bin/arm-iwmmxt-linux-gnueabi-g++ -o vr_test main.cpp -I. -ldl -DTESTPROGRAM -DST

clean:
	rm vr_test
