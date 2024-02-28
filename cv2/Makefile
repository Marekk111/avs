cv2_arping:
	gcc -Wno-address-of-packed-member  -I../ -Wall cv02-arping.c -o cv02_arping
	sudo setcap cap_net_raw+ep cv02_arping 
clean:
	rm -rf cv02_arping
