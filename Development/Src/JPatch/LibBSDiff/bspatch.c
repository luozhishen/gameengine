/*-
 * Copyright 2003-2005 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions 
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if 0
__FBSDID("$FreeBSD: src/usr.bin/bsdiff/bspatch/bspatch.c,v 1.1 2005/08/06 01:59:06 cperciva Exp $");
#endif

#include <sys/types.h>
#include "bzlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

static off_t offtin(u_char *buf)
{
	off_t y;

	y=buf[7]&0x7F;
	y=y*256;y+=buf[6];
	y=y*256;y+=buf[5];
	y=y*256;y+=buf[4];
	y=y*256;y+=buf[3];
	y=y*256;y+=buf[2];
	y=y*256;y+=buf[1];
	y=y*256;y+=buf[0];

	if(buf[7]&0x80) y=-y;

	return y;
}

int bspatch(u_char* old, off_t oldsize, FILE* f, u_char** new_out, off_t* newsize_out)
{
	size_t offset;
	BZFILE * cpfbz2=NULL, * dpfbz2=NULL, * epfbz2=NULL;
	int cbz2err, dbz2err, ebz2err;
	u_char* newp=NULL;
	off_t newsize;
	off_t bzctrllen,bzdatalen;
	u_char header[32],buf[8];
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;

	offset = ftell(f);

	/*
	File format:
		0	8	"BSDIFF40"
		8	8	X
		16	8	Y
		24	8	sizeof(newfile)
		32	X	bzip2(control block)
		32+X	Y	bzip2(diff block)
		32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	/* Read header */
	if (fread(header, 1, 32, f) < 32) {
		if (feof(f)) {
			printf("Corrupt patch\n");
		} else {
			printf("fread(%s)", "patch_file");
		}
		goto error;
	}

	/* Read lengths from header */
	if(memcmp(header, "BSDIFFXX", 8)!=0) {
		printf("Corrupt patch\n");
		goto error;
	}
	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0)) {
		printf("Corrupt patch\n");
		goto error;
	}

	newp = malloc(newsize);
	if(!newp) goto error;

	if ((cpfbz2 = BZ2_bzReadOpen(&cbz2err, f, 0, 0, NULL, 0)) == NULL) {
		printf("BZ2_bzReadOpen, bz2err = %d", cbz2err);
		goto error;
	}
	if (fseek(f, offset + 32 + bzctrllen, SEEK_SET)) {
		printf("fseeko(%s, %lld)", "patch_file", (long long)(offset + 32 + bzctrllen));
		goto error;
	}
	if ((dpfbz2 = BZ2_bzReadOpen(&dbz2err, f, 0, 0, NULL, 0)) == NULL) {
		printf("BZ2_bzReadOpen, bz2err = %d", dbz2err);
		goto error;
	}
	if (fseek(f, offset + 32 + bzctrllen + bzdatalen, SEEK_SET)) {
		printf("fseeko(%s, %lld)", "patch_file", (long long)(32 + bzctrllen + bzdatalen));
		goto error;
	}
	if ((epfbz2 = BZ2_bzReadOpen(&ebz2err, f, 0, 0, NULL, 0)) == NULL) {
		printf("BZ2_bzReadOpen, bz2err = %d", ebz2err);
		goto error;
	}

	oldpos=0;newpos=0;
	while(newpos<newsize) {
		/* Read control data */
		for(i=0;i<=2;i++) {
			lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
			if ((lenread < 8) || ((cbz2err != BZ_OK) &&
			    (cbz2err != BZ_STREAM_END))) {
				printf("Corrupt patch\n");
				goto error;
			}
			ctrl[i]=offtin(buf);
		};

		/* Sanity-check */
		if(newpos+ctrl[0]>newsize) {
			printf("Corrupt patch\n");
			goto error;
		}

		/* Read diff string */
		lenread = BZ2_bzRead(&dbz2err, dpfbz2, newp + newpos, ctrl[0]);
		if ((lenread < ctrl[0]) ||
		    ((dbz2err != BZ_OK) && (dbz2err != BZ_STREAM_END))) {
			printf("Corrupt patch\n");
			goto error;
		}

		/* Add old data to diff string */
		for(i=0;i<ctrl[0];i++)
			if((oldpos+i>=0) && (oldpos+i<oldsize))
				newp[newpos+i]+=old[oldpos+i];

		/* Adjust pointers */
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize) {
			printf("Corrupt patch\n");
			goto error;
		}

		/* Read extra string */
		lenread = BZ2_bzRead(&ebz2err, epfbz2, newp + newpos, ctrl[1]);
		if ((lenread < ctrl[1]) ||
		    ((ebz2err != BZ_OK) && (ebz2err != BZ_STREAM_END))) {
			printf("Corrupt patch\n");
			goto error;
		}

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];
	};

	/* Clean up the bzip2 reads */
	BZ2_bzReadClose(&cbz2err, cpfbz2);
	BZ2_bzReadClose(&dbz2err, dpfbz2);
	BZ2_bzReadClose(&ebz2err, epfbz2);

	*new_out = newp;
	*newsize_out = newsize;
	return 0;

error:
	if(cpfbz2) BZ2_bzReadClose(&cbz2err, cpfbz2);
	if(dpfbz2) BZ2_bzReadClose(&dbz2err, dpfbz2);
	if(epfbz2) BZ2_bzReadClose(&ebz2err, epfbz2);
	if(newp) free(newp);
	return -1;
}
