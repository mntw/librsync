#include <cstdio>
#include <librsync.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

void rsync_signature_work(struct rsync_signature *rs, const char *data, size_t len);
void rsync_signature_free(struct rsync_signature *rs);


struct rsync_signature {
    struct rs_buffers_s buffers;
    struct rs_job *job;
    char *output;
    size_t allocated_output;
};

struct rsync_signature * rsync_signature_new() {
    struct rsync_signature * rs =  (rsync_signature *) malloc(sizeof(*rs));
//    if (!rs ) fatal_error("Cannot allocate");
    rs->allocated_output = 4096;
    rs->output = (char *) malloc(rs->allocated_output);
//    if (!rs->output) fatal_error("Cannot allocate");
    rs->buffers.next_out = rs->output;
    rs->buffers.avail_out = rs->allocated_output;
    rs->buffers.avail_in = 0;
    rs->buffers.eof_in = 0;
    rs->job = rs_sig_begin(RS_DEFAULT_BLOCK_LEN, RS_DEFAULT_STRONG_LEN);
    //assert(rs->job != 0);
    return rs;
}

void signature() {
    FILE *fin = stdin;
    char buffer[4096];
    int len;

    struct rsync_signature *rs;

    rs = rsync_signature_new();

    while((len = fread(buffer, 1, sizeof buffer, fin)) > 0) {
        rsync_signature_work(rs, buffer, len);
    }

    rsync_signature_work(rs, buffer, 0);

    fwrite(rs->output, 1, (rs->buffers.next_out - rs->output), stdout);

    rsync_signature_free(rs);
}

void rsync_signature_work(struct rsync_signature *rs, const char *data, size_t len) {
    rs_result res;
    //assert(rs->buffers.avail_in == 0);
    rs->buffers.next_in = (char *) data;
    rs->buffers.avail_in = len;
    if(len == 0) rs->buffers.eof_in = 1;
    do {
        res = rs_job_iter(rs->job, &rs->buffers);
        //assert(res == RS_DONE || res == RS_BLOCKED);
        if (rs->buffers.avail_out == 0) {
            int written = rs->buffers.next_out - rs->output;
            const int increment = 4096;
            rs->allocated_output += increment;
            rs->output = (char *) realloc(rs->output, rs->allocated_output);
//            if (!rs->output) fatal_error("Cannot realloc");
            rs->buffers.next_out = rs->output + written;
            rs->buffers.avail_out = increment;
        }
    } while (rs->buffers.avail_in > 0 || (rs->buffers.eof_in && res != RS_DONE));
}

void rsync_signature_free(struct rsync_signature *rs) {
    free(rs->output);
    rs_job_free(rs->job);
    free(rs);
}

size_t rsync_signature_size(const struct rsync_signature *rs) {
    return rs->buffers.next_out - rs->output;
}

int main(int argc, char * argv[]) {
	signature();
	return 0;
}
