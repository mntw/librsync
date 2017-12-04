#include <iostream>
#include <stdio.h>
#include <string.h>
#include <openssl/md4.h>
#include <string>
#include <numeric>

#define ROLLSUM_CHAR_OFFSET 31

#define DO1(buf,i)  {s1 += int(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

std::string make_md4_block(const std::string & myvec) {

	//http://www.askyb.com/cpp/openssl-md4-hashing-example-in-cpp/
//	unsigned char digest[MD4_DIGEST_LENGTH];

	std::string res;
	res.resize(MD4_DIGEST_LENGTH,'a');
	MD4(reinterpret_cast<const unsigned char*>(myvec.data()),
			myvec.size(),
			reinterpret_cast<unsigned char *>(res.data()) );//(unsigned char*)&str, strlen(str), (unsigned char*)&digest);    
	return res;
}

//void RollsumUpdate(Rollsum *sum, const unsigned char *buf, size_t len) {
//    size_t n = len;
//    uint_fast16_t s1 = sum->s1;
//    uint_fast16_t s2 = sum->s2;
//
//    while (n >= 16) {
//        DO16(buf);
//        buf += 16;
//        n -= 16;
//    }
//    while (n != 0) {
//        s1 += *buf++;
//        s2 += s1;
//        n--;
//    }
//    /* Increment s1 and s2 by the amounts added by the char offset. */
//    s1 += len * ROLLSUM_CHAR_OFFSET;
//    s2 += ((len * (len + 1)) / 2) * ROLLSUM_CHAR_OFFSET;
//    sum->count += len;          /* Increment sum count. */
//    sum->s1 = s1;
//    sum->s2 = s2;
//}


uint32_t RollsumDigest(const std::string & buf, size_t len) {
	uint_fast16_t s1 = 0; 
	uint_fast16_t s2 = 0; 
	size_t n = len;
	int result [16] = buf;
        while (n >= 16) {
	    std::partial_sum (buf[n], buf[n+16], result);
	    s1 += std::accumulate(buf[i], buf[i+16],0);
	    s2 += std::accumulate(result, result + 16, 0);
            n -= 16;
        }
        while (n != 0) {
            s1 += *buf++;
            s2 += s1;
            n--;
        }
        /* Increment s1 and s2 by the amounts added by the char offset. */
        s1 += len * ROLLSUM_CHAR_OFFSET;
        s2 += ((len * (len + 1)) / 2) * ROLLSUM_CHAR_OFFSET;
        return ((uint32_t)s2 << 16) | ((uint32_t)s1 & 0xffff);
}


int main (int argc, char ** argv) {

//	char mdString[33];
//
//	for(int i = 0; i < MD4_DIGEST_LENGTH; i++) {
//		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
//	}
//
//	printf("md4 digest: %s\n", mdString);

	std::string input = "hello world\n";
	auto res = make_md4_block(input);
		std::cout<<res;
	RollsumDigest(input, input.size());

	return 0;
}
