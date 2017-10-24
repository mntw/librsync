all:
	g++ -o sdp sync_delta_patch.cpp -lrsync -Llibrsync
test: clear
	fallocate -l 200M FILE
	cp FILE FILE.new
	echo 1 >> FILE.new
	md5sum FILE
	md5sum FILE.new
	./sdp FILE FILE.new
	md5sum FILE
	md5sum FILE.new
clean: clear
	rm -rf sdp 
clear:
	rm -rf FILE*

