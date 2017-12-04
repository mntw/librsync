all: sdp sign delta patch
sdp:
	g++ -o bin/sdp src/librsfunc.cpp src/sync_delta_patch.cpp -lrsync -Llibrsync
sign:
	g++ -o bin/sign src/librsfunc.cpp src/sync.cpp -lrsync -Llibrsync
delta:
	g++ -o bin/delta src/librsfunc.cpp src/delta.cpp -lrsync -Llibrsync
patch:
	g++ -o bin/patch src/librsfunc.cpp src/patch.cpp -lrsync -Llibrsync
ownsign:
	g++ -std=c++17 -o bin/own_sign src/own_sign.cpp -lcrypto 
testSDP: clear
	fallocate -l 200M test/FILE
	cp test/FILE test/FILE.new
	echo 1 >> test/FILE.new
	md5sum test/FILE
	md5sum test/FILE.new
	bin/sdp test/FILE test/FILE.new
	md5sum test/FILE
	md5sum test/FILE.new
testSDPsep: all clear
	fallocate -l 200M test/FILE
	cp test/FILE test/FILE.new
	echo 1 >> test/FILE.new
	md5sum test/FILE
	md5sum test/FILE.new
	bin/sync test/FILE 
	bin/delta test/FILE test/FILE.new 
	bin/patch test/FILE 
	md5sum test/FILE.new
	md5sum test/FILE.upd
testSDPsepPy: all clear
	fallocate -l 200M test/FILE
	cp test/FILE test/FILE.new
	echo 1 >> test/FILE.new
	md5sum test/FILE
	md5sum test/FILE.new
	bin/weakhs test/FILE 
	bin/delta test/FILE test/FILE.new 
	bin/patch test/FILE 
	md5sum test/FILE.new
	md5sum test/FILE.upd
t: 
	bin/sdp FILE FILE.new
clean: clear
	rm bin/!("weakhs") 
clear:
	rm -rf test/FILE*

